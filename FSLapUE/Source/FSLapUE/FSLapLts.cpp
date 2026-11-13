#include "FSLapLts.h"

#include "Dom/JsonObject.h"
#include "HAL/FileManager.h"
#include "HAL/PlatformMisc.h"
#include "HAL/PlatformProcess.h"
#include "HAL/PlatformTime.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "Misc/SecureHash.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"

const TCHAR* const FFSLapLts::Models[4] = { TEXT(""), TEXT("pointMass"), TEXT("singleTrack"), TEXT("dualTrack") };
const TCHAR* const FFSLapLts::ModelLabels[4] = { TEXT("stored lap"), TEXT("point mass"), TEXT("single track"), TEXT("dual track") };

namespace
{
	FString EnvOr(const TCHAR* Name, const FString& Default)
	{
		const FString Value = FPlatformMisc::GetEnvironmentVariable(Name);
		return Value.IsEmpty() ? Default : Value;
	}

	FString MatlabExe()
	{
		const FString Set = FPlatformMisc::GetEnvironmentVariable(TEXT("FSLAP_MATLAB"));
		if (!Set.IsEmpty())
		{
			return Set;
		}
		TArray<FString> Releases;   // R2024b, R2025a ...: the newest one
		IFileManager::Get().FindFiles(Releases, TEXT("C:/Program Files/MATLAB/R20*"), false, true);
		Releases.Sort();
		return Releases.Num() ? FString::Printf(TEXT("C:/Program Files/MATLAB/%s/bin/matlab.exe"), *Releases.Last()) : FString();
	}

	bool Numbers(const TSharedPtr<FJsonObject>& Object, const TCHAR* Field, TArray<double>& Out)
	{
		const TArray<TSharedPtr<FJsonValue>>* Values = nullptr;
		if (!Object->TryGetArrayField(Field, Values))
		{
			return false;
		}
		Out.Reset(Values->Num());
		for (const TSharedPtr<FJsonValue>& V : *Values)
		{
			Out.Add(V->AsNumber());
		}
		return Out.Num() > 1;
	}
}

FFSLapLts::~FFSLapLts()
{
	if (bWorkerStarted)
	{
		FFileHelper::SaveStringToFile(TEXT(""), *(Folder(TEXT("queue")) / TEXT("stop")));   // the worker ends after its current job
	}
}

FString FFSLapLts::Folder(const TCHAR* Sub) const
{
	return FPaths::ConvertRelativePathToFull(FPaths::ProjectSavedDir() / TEXT("LTS") / Sub);
}

bool FFSLapLts::WorkerAlive() const
{
	// the worker rewrites worker.json every second while it waits
	const FDateTime Beat = IFileManager::Get().GetTimeStamp(*(Folder(TEXT("queue")) / TEXT("worker.json")));
	return Beat != FDateTime::MinValue() && (FDateTime::UtcNow() - Beat).GetTotalSeconds() < 15;
}

bool FFSLapLts::StartWorker()
{
	const FString Root = EnvOr(TEXT("FSLAP_LTS_ROOT"), TEXT("D:/VD/laptimesimulation"));
	const FString Matlab = MatlabExe();
	if (!FPaths::FileExists(Root / TEXT("+lts/+viewer/worker.m")))
	{
		StatusText = TEXT("LTS not found (set FSLAP_LTS_ROOT)");
		return false;
	}
	if (Matlab.IsEmpty() || !FPaths::FileExists(Matlab))
	{
		StatusText = TEXT("MATLAB not found (set FSLAP_MATLAB)");
		return false;
	}
	// the LTS's own folders on the path, as its project would set them, without opening the project (that can write there)
	const FString Script = FString::Printf(
		TEXT("r='%s'; addpath(r, fullfile(r,'config'), fullfile(r,'vehicles'), fullfile(r,'data')); ")
		TEXT("t=dir(fullfile(r,'tools')); for k=1:numel(t), if t(k).isdir && t(k).name(1)~='.', addpath(fullfile(r,'tools',t(k).name)); end, end; ")
		TEXT("lts.viewer.worker('%s', IdleMinutes=30)"), *Root, *Folder(TEXT("queue")));
	const FString Args = FString::Printf(TEXT("-batch \"%s\""), *Script);
	FProcHandle Proc = FPlatformProcess::CreateProc(*Matlab, *Args, true, true, true, nullptr, 0, *Folder(TEXT("queue")), nullptr);
	if (!Proc.IsValid())
	{
		StatusText = TEXT("could not start MATLAB");
		return false;
	}
	FPlatformProcess::CloseProc(Proc);
	bWorkerStarted = true;
	return true;
}

void FFSLapLts::Request(int32 Model, const TArray<TPair<FString, double>>& Overrides)
{
	check(Model >= 1 && Model <= 3);
	FString Pairs;
	for (const TPair<FString, double>& O : Overrides)
	{
		Pairs += FString::Printf(TEXT("%s%s: %.6g"), Pairs.IsEmpty() ? TEXT("") : TEXT(", "), *O.Key, O.Value);
	}
	// the job id names the result folder, so the same setup and model is computed once
	const FString Key = FString(Models[Model]) + TEXT("|") + Pairs;
	JobId = FString::Printf(TEXT("fslap_%s_%s"), Models[Model], *FMD5::HashAnsiString(*Key).Left(10));
	JobStarted = FPlatformTime::Seconds();
	NextPoll = 0;
	const FString Output = Folder(TEXT("runs")) / JobId;
	if (FPaths::FileExists(Output / TEXT("case_001.json")))
	{
		StatusText = TEXT("cached");
		return;
	}
	if (FPaths::DirectoryExists(Output))
	{
		IFileManager::Get().DeleteDirectory(*Output, false, true);   // a run that did not finish: runJob needs a new folder
	}
	IFileManager::Get().MakeDirectory(*Folder(TEXT("queue")), true);
	IFileManager::Get().MakeDirectory(*Folder(TEXT("runs")), true);
	FString Overrides_;
	for (const TPair<FString, double>& O : Overrides)
	{
		Overrides_ += FString::Printf(TEXT("%s\"%s\": %.9g"), Overrides_.IsEmpty() ? TEXT("") : TEXT(", "), *O.Key, O.Value);
	}
	const FString Job = FString::Printf(TEXT("{\"schema\": \"homo-lts.job.v1\", \"id\": \"%s\", \"name\": \"FS Lap setup, %s\", \"model\": \"%s\", ")
		TEXT("\"track\": \"endurance\", \"outputFolder\": \"%s\", \"overrides\": {%s}}"),
		*JobId, ModelLabels[Model], Models[Model], *Output.Replace(TEXT("\\"), TEXT("/")), *Overrides_);
	const FString Queued = Folder(TEXT("queue")) / (JobId + TEXT(".queued.json"));
	FFileHelper::SaveStringToFile(Job, *(Queued + TEXT(".tmp")));
	IFileManager::Get().Move(*Queued, *(Queued + TEXT(".tmp")));   // whole files only: the worker may look at any moment
	if (!WorkerAlive() && !StartWorker())
	{
		JobId.Empty();
		return;
	}
	StatusText = TEXT("starting");
}

bool FFSLapLts::Poll(FLap& Out)
{
	const double Now = FPlatformTime::Seconds();
	if (JobId.IsEmpty() || Now < NextPoll)
	{
		return false;
	}
	NextPoll = Now + 0.5;   // file checks only, twice a second
	const FString Run = Folder(TEXT("runs")) / JobId / TEXT("case_001.json");
	FString Text;
	if (FPaths::FileExists(Run) && FFileHelper::LoadFileToString(Text, *Run))
	{
		JobId.Empty();
		if (!ParseRun(Text, Out))
		{
			StatusText = TEXT("unreadable result");
			return false;
		}
		StatusText.Empty();
		return true;
	}
	FString Done;
	if (FFileHelper::LoadFileToString(Done, *(Folder(TEXT("queue")) / (JobId + TEXT(".done.json")))) && !Done.Contains(TEXT("\"status\":\"done\"")))
	{
		StatusText = TEXT("LTS failed, see Saved/LTS/queue/") + JobId + TEXT(".log");
		JobId.Empty();
		return false;
	}
	const int32 Seconds = FMath::RoundToInt32(Now - JobStarted);
	StatusText = (WorkerAlive() ? FString::Printf(TEXT("running %d s"), Seconds) : FString::Printf(TEXT("starting MATLAB %d s"), Seconds));
	return false;
}

bool FFSLapLts::ParseRun(const FString& Json, FLap& Out)
{
	TSharedPtr<FJsonObject> Root;
	if (!FJsonSerializer::Deserialize(TJsonReaderFactory<>::Create(Json), Root) || !Root.IsValid())
	{
		return false;
	}
	const TSharedPtr<FJsonObject>* Lap = nullptr;
	const TSharedPtr<FJsonObject>* Summary = nullptr;
	const TSharedPtr<FJsonObject>* Track = nullptr;
	if (!Root->TryGetObjectField(TEXT("lap"), Lap) || !Root->TryGetObjectField(TEXT("summary"), Summary) || !Root->TryGetObjectField(TEXT("track"), Track))
	{
		return false;
	}
	if (!Numbers(*Lap, TEXT("distance_m"), Out.Distance) || !Numbers(*Lap, TEXT("time_s"), Out.Time) || !Numbers(*Lap, TEXT("speed_mps"), Out.Speed)
		|| Out.Distance.Num() != Out.Time.Num())
	{
		return false;
	}
	Out.LapTime = (*Summary)->GetNumberField(TEXT("lapTime_s"));
	Out.Length = (*Track)->GetNumberField(TEXT("length_m"));
	Out.Model = Root->GetStringField(TEXT("model"));
	return Out.LapTime > 0 && Out.Length > 0;
}
