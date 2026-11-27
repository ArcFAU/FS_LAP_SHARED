#include "FSLapLog.h"

#include "Async/Async.h"
#include "Misc/DateTime.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"

const TCHAR* const FFSLapLog::Columns = TEXT("time_s,distance_m,x_m,y_m,heading_rad,vx_mps,vy_mps,speed_mps,yawRate_radps,ax_mps2,ay_mps2,")
	TEXT("steerInput,steer,roadWheelAngle_rad,throttle,brake,handbrake,power_kW,lateralOffset_m,slipFront,slipRear,grass");

namespace
{
	constexpr int32 NumColumns = 22;

	FString Array(const TArray<double>& Rows, int32 Column, int32 Every, int32 Decimals, double Sign = 1.0)
	{
		FString Out = TEXT("[");
		for (int32 Row = 0; Row * NumColumns < Rows.Num(); Row += Every)
		{
			Out += FString::Printf(TEXT("%s%.*f"), Row ? TEXT(",") : TEXT(""), Decimals, Sign * Rows[Row * NumColumns + Column]);
		}
		return Out + TEXT("]");
	}
}

void FFSLapLog::Begin(const FString& Mode, const FString& SetupJson, const FString& Reference, const FString& Kernel)
{
	KernelName = Kernel;
	Rows.Reset();
	Session = FDateTime::Now().ToString(TEXT("%Y-%m-%d_%H%M%S")) + TEXT("_") + Mode;
	ModeName = Mode;
	Setup = SetupJson;
	ReferenceJson = Reference;
	SeenLaps = 0;
	// VERSION.txt is written by the sync into the shared repository (commit and date); the private repository has none
	if (!FFileHelper::LoadFileToString(Version, *(FPaths::ProjectDir() / TEXT("../VERSION.txt"))))
	{
		Version = TEXT("development");
	}
	Version = Version.TrimStartAndEnd().Replace(TEXT("\""), TEXT("")).Replace(TEXT("\\"), TEXT(""));
}

void FFSLapLog::Sample(const HoMoLap::Simulation& Sim, const HoMoLap::Input* Human)
{
	if (Session.IsEmpty())
	{
		return;
	}
	if (Sim.laps.size() > SeenLaps)   // the step that crossed the line finished a lap
	{
		if (Rows.Num())
		{
			Flush(Sim, Sim.laps.back());
		}
		Rows.Reset();
		SeenLaps = Sim.laps.size();
	}
	if (!Sim.IsLapActive() || (Rows.Num() && Sim.CurrentLapTime() < Rows[Rows.Num() - NumColumns]))
	{
		Rows.Reset();   // out lap, or the lap started again
		if (!Sim.IsLapActive())
		{
			return;
		}
	}
	const HoMoLap::Car& C = Sim.c;
	const double Row[NumColumns] = { Sim.CurrentLapTime(), Sim.LapProgress(), C.x, C.y, C.h, C.vx, C.vy, FMath::Sqrt(C.vx * C.vx + C.vy * C.vy), C.r, C.ax, C.ay,
		Human ? Human->steer : C.st, C.st, C.delta, C.thr, C.brk, Human && Human->hb ? 1.0 : 0.0, C.power / 1000.0, C.lat,
		C.slipF ? 1.0 : 0.0, C.slipR ? 1.0 : 0.0, C.grass ? 1.0 : 0.0 };
	Rows.Append(Row, NumColumns);
}

void FFSLapLog::Flush(const HoMoLap::Simulation& Sim, const HoMoLap::LapResult& Lap)
{
	const FString Folder = FPaths::ConvertRelativePathToFull(FPaths::ProjectSavedDir() / TEXT("Runs") / Session);
	const FString Base = Folder / FString::Printf(TEXT("lap_%02d"), Lap.n);
	LastFile = Base + TEXT(".csv");
	// the track's curvature at each logged point, for the run format
	TArray<double> Curvature;
	const HoMoLap::Track& T = Sim.T;
	for (int32 Row = 0; Row * NumColumns < Rows.Num(); ++Row)
	{
		const double S = Rows[Row * NumColumns + 1];
		Curvature.Add(T.N ? T.K[FMath::Clamp(static_cast<int32>(S / FMath::Max(T.LEN / T.N, 1e-6)), 0, T.N - 1)] : 0.0);
	}
	const double TrackLength = T.LEN;
	const FString Header = FString::Printf(TEXT("{\"schema\": \"homo-lts.run.v1\", \"created\": \"%s\", \"model\": \"fslapDriven\", \"calculationKernel\": \"%s\", \"fslapVersion\": \"%s\", ")
		TEXT("\"vehicleName\": \"FAUmax tau (FS Lap)\", \"track\": {\"name\": \"%s\", \"length_m\": %.4f, \"isClosed\": true}, "),
		*FDateTime::Now().ToString(TEXT("%Y-%m-%d %H:%M:%S")), *KernelName, *Version,
		// FSG: the LTS's own track file, as HoMo-LTS names it; the others their own name (FS Czech and FS France laps were
		// written as FSG's until 2026-11-27, because they have a reference lap too)
		*(FCStringAnsi::Strcmp(T.id, "fsg") == 0 ? FString(TEXT("A_FSGend_3.mat")) : FString(UTF8_TO_TCHAR(T.name))), TrackLength);
	const FString Result = FString::Printf(TEXT("{\"lap\": %d, \"time_s\": %.4f, \"rawTime_s\": %.4f, \"penalty_s\": %.1f, \"split1_s\": %.4f, \"split2_s\": %.4f, \"valid\": %s, \"cones\": %d, \"offCourse\": %d}"),
		Lap.n, Lap.time, Lap.raw, Lap.pen, Lap.split1, Lap.split2, Lap.valid ? TEXT("true") : TEXT("false"), Lap.cones, Lap.offCourse);
	const FString Mode = ModeName, SetupJson = Setup, Reference = ReferenceJson.IsEmpty() ? TEXT("null") : ReferenceJson;
	const double LapTime = Lap.raw;   // the official time, without penalties: the last row is the step that crossed the line
	// the strings are built and written on a worker thread: a lap is about 50,000 rows
	Async(EAsyncExecution::ThreadPool, [Rows = Rows, Curvature = MoveTemp(Curvature), Folder, Base, Header, Result, Mode, SetupJson, Reference, LapTime]()
	{
		const int32 Count = Rows.Num() / NumColumns;
		FString Csv = FString(Columns) + TEXT("\n");
		Csv.Reserve(Count * 200);
		for (int32 Row = 0; Row < Count; ++Row)
		{
			for (int32 Col = 0; Col < NumColumns; ++Col)
			{
				Csv += FString::Printf(TEXT("%s%.6g"), Col ? TEXT(",") : TEXT(""), Rows[Row * NumColumns + Col]);
			}
			Csv += TEXT("\n");
		}
		double Top = 0, Sum = 0;
		for (int32 Row = 0; Row < Count; ++Row)
		{
			Top = FMath::Max(Top, Rows[Row * NumColumns + 7]);
			Sum += Rows[Row * NumColumns + 7];
		}
		const int32 Every = 6;   // 600 Hz to 100 Hz for the run format
		// the LTS counts curvature and lateral acceleration the other way round: lap.* follows the LTS, fslap.channels stay as the game logs them
		FString CurvatureArray = TEXT("[");
		for (int32 Row = 0; Row < Count; Row += Every)
		{
			CurvatureArray += FString::Printf(TEXT("%s%.6f"), Row ? TEXT(",") : TEXT(""), -Curvature[Row]);
		}
		CurvatureArray += TEXT("]");
		FString Channels;
		TArray<FString> Names;
		FString(Columns).ParseIntoArray(Names, TEXT(","));
		for (int32 Col = 0; Col < NumColumns; ++Col)
		{
			Channels += FString::Printf(TEXT("%s\"%s\": %s"), Col ? TEXT(", ") : TEXT(""), *Names[Col], *Array(Rows, Col, Every, 5));
		}
		const FString Json = Header
			+ FString::Printf(TEXT("\"summary\": {\"lapTime_s\": %.4f, \"maximumSpeed_mps\": %.4f, \"meanSpeed_mps\": %.4f}, "), LapTime, Top, Count ? Sum / Count : 0)
			+ TEXT("\"lap\": {\"distance_m\": ") + Array(Rows, 1, Every, 3) + TEXT(", \"time_s\": ") + Array(Rows, 0, Every, 4)
			+ TEXT(", \"speed_mps\": ") + Array(Rows, 7, Every, 4) + TEXT(", \"curvature_1pm\": ") + CurvatureArray
			+ TEXT(", \"lateralAcceleration_mps2\": ") + Array(Rows, 10, Every, 4, -1.0) + TEXT("}, ")
			+ TEXT("\"fslap\": {\"mode\": \"") + Mode + TEXT("\", \"result\": ") + Result + TEXT(", \"setup\": ") + SetupJson
			+ TEXT(", \"reference\": ") + Reference + TEXT(", \"sampleRate_Hz\": 100, \"channels\": {") + Channels + TEXT("}}}");
		FFileHelper::SaveStringToFile(Csv, *(Base + TEXT(".csv")));
		FFileHelper::SaveStringToFile(Json, *(Base + TEXT(".run.json")));
	});
}
