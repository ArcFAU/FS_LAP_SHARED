#include "FSLapGpuBench.h"

#include "Camera/CameraActor.h"
#include "CoreGlobals.h"
#include "DynamicRHI.h"
#include "Engine/Engine.h"
#include "Engine/GameViewportClient.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "HAL/PlatformTime.h"
#include "Misc/CommandLine.h"
#include "Misc/FileHelper.h"
#include "Misc/Parse.h"
#include "Misc/Paths.h"
#include "RHI.h"
#include "RHIGlobals.h"
#include "RenderTimer.h"
#include "RenderUtils.h"
#include "SceneManagement.h"
#include "UnrealClient.h"

#if WITH_EDITOR
#include "ShaderCompiler.h"
#endif

DEFINE_LOG_CATEGORY_STATIC(LogFSLapBench, Log, All);

namespace
{
	constexpr double StartSeconds = 5.0;
	constexpr double WarmupSeconds = 15.0;
	constexpr double MeasureSeconds = 10.0;
	constexpr double ShotSeconds = 3.0;
	constexpr double GiveUpSeconds = 7200.0;

	struct FBenchCase
	{
		const TCHAR* Name;
		TArray<const TCHAR*> Commands;
	};

	// Each case builds on the state the one before left behind.
	const TArray<FBenchCase>& Cases()
	{
		static const TArray<FBenchCase> List = {
			{ TEXT("1_Plain"), { TEXT("r.DynamicGlobalIlluminationMethod 0"), TEXT("r.ReflectionMethod 2"), TEXT("r.Nanite 0"), TEXT("r.Shadow.Virtual.Enable 0") } },
			{ TEXT("2_Nanite"), { TEXT("r.Nanite 1") } },
			{ TEXT("3_Nanite_VSM"), { TEXT("r.Shadow.Virtual.Enable 1") } },
			{ TEXT("4_LumenMedium_ShadowMaps"), { TEXT("r.Shadow.Virtual.Enable 0"), TEXT("sg.GlobalIlluminationQuality 1"), TEXT("sg.ReflectionQuality 1"), TEXT("r.DynamicGlobalIlluminationMethod 1"), TEXT("r.ReflectionMethod 1") } },
			{ TEXT("5_LumenMedium_VSM"), { TEXT("r.Shadow.Virtual.Enable 1") } },
			{ TEXT("6_LumenHigh_VSM"), { TEXT("sg.GlobalIlluminationQuality 2"), TEXT("sg.ReflectionQuality 2") } },
		};
		return List;
	}

	double Average(const TArray<double>& Values)
	{
		double Sum = 0.0;
		for (double Value : Values)
		{
			Sum += Value;
		}
		return Values.Num() ? Sum / Values.Num() : 0.0;
	}

	double Percentile(TArray<double> Values, double Fraction)
	{
		if (Values.IsEmpty())
		{
			return 0.0;
		}
		Values.Sort();
		return Values[FMath::Clamp(FMath::FloorToInt32(Fraction * Values.Num()), 0, Values.Num() - 1)];
	}
}

bool UFSLapGpuBench::ShouldCreateSubsystem(UObject* Outer) const
{
	return Super::ShouldCreateSubsystem(Outer) && FParse::Param(FCommandLine::Get(), TEXT("FSLapGpuBench"));
}

bool UFSLapGpuBench::DoesSupportWorldType(const EWorldType::Type WorldType) const
{
	return WorldType == EWorldType::Game;
}

TStatId UFSLapGpuBench::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UFSLapGpuBench, STATGROUP_Tickables);
}

FString UFSLapGpuBench::OutputDir() const
{
	return FPaths::ConvertRelativePathToFull(FPaths::ProjectSavedDir() / TEXT("GpuBench"));
}

void UFSLapGpuBench::Exec(const FString& Command)
{
	UE_LOG(LogFSLapBench, Display, TEXT("Exec: %s"), *Command);
	GEngine->Exec(GetWorld(), *Command);
}

void UFSLapGpuBench::ApplyCase(int32 Index)
{
	for (const TCHAR* Command : Cases()[Index].Commands)
	{
		Exec(Command);
	}
	UE_LOG(LogFSLapBench, Display, TEXT("Case %s: warming up"), Cases()[Index].Name);
	FrameMs.Reset();
	GpuMs.Reset();
	GameMs.Reset();
	RenderMs.Reset();
	Phase = EPhase::Warmup;
	PhaseStart = FPlatformTime::Seconds();
}

void UFSLapGpuBench::FinishCase()
{
	const FString Name = Cases()[CaseIndex].Name;
	const double Avg = Average(FrameMs);
	const FString Row = FString::Printf(TEXT("%s,%d,%.1f,%.2f,%.2f,%.2f,%.2f,%.2f"), *Name, FrameMs.Num(), Avg > 0.0 ? 1000.0 / Avg : 0.0, Avg,
		Percentile(FrameMs, 0.95), Average(GpuMs), Average(GameMs), Average(RenderMs));
	Rows.Add(Row);
	UE_LOG(LogFSLapBench, Display, TEXT("Result: %s"), *Row);

	FScreenshotRequest::RequestScreenshot(OutputDir() / (Name + TEXT(".png")), false, false);
	Phase = EPhase::Shot;
	PhaseStart = FPlatformTime::Seconds();
}

void UFSLapGpuBench::Finish()
{
	FString Text = FString::Printf(TEXT("# adapter=%s\n# maxShaderPlatform=%d rayTracing=%d naniteSupported=%d naniteUsable=%d lumenGI=%d distanceFields=%d viewport=%s\n"),
		*GRHIAdapterName, (int32)GMaxRHIShaderPlatform, GRHISupportsRayTracing ? 1 : 0, DoesPlatformSupportNanite(GMaxRHIShaderPlatform) ? 1 : 0, UseNanite(GMaxRHIShaderPlatform) ? 1 : 0,
		DoesPlatformSupportLumenGI(GMaxRHIShaderPlatform) ? 1 : 0, DoesProjectSupportDistanceFields() ? 1 : 0,
		GEngine->GameViewport && GEngine->GameViewport->Viewport ? *GEngine->GameViewport->Viewport->GetSizeXY().ToString() : TEXT("?"));
	Text += TEXT("case,samples,fps,frame_ms,frame_ms_p95,gpu_ms,game_ms,render_ms\n");
	Text += FString::Join(Rows, TEXT("\n")) + TEXT("\n");

	const FString File = OutputDir() / TEXT("results.csv");
	FFileHelper::SaveStringToFile(Text, *File);
	UE_LOG(LogFSLapBench, Display, TEXT("Wrote %s"), *File);
	Phase = EPhase::Done;
	RequestEngineExit(TEXT("FSLap GPU bench finished"));
}

void UFSLapGpuBench::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	const double Now = FPlatformTime::Seconds();
	const double FrameTime = LastTime > 0.0 ? (Now - LastTime) * 1000.0 : 0.0;
	LastTime = Now;
	if (BenchStart == 0.0)
	{
		BenchStart = PhaseStart = Now;
		UE_LOG(LogFSLapBench, Display, TEXT("GPU bench started"));
	}

	if (Phase != EPhase::Done && Now - BenchStart > GiveUpSeconds)
	{
		UE_LOG(LogFSLapBench, Error, TEXT("Gave up after %.0f s"), GiveUpSeconds);
		Finish();
		return;
	}

	bool bCompiling = false;
#if WITH_EDITOR
	bCompiling = GShaderCompilingManager && GShaderCompilingManager->IsCompiling();
#endif

	switch (Phase)
	{
	case EPhase::Start:
		if (Now - PhaseStart >= StartSeconds)
		{
			// A fixed camera: with the default pawn, moving the mouse during the run changes what is measured.
			if (APlayerController* Player = GetWorld()->GetFirstPlayerController())
			{
				FActorSpawnParameters Spawn;
				Spawn.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
				Player->SetViewTarget(GetWorld()->SpawnActor<ACameraActor>(FVector(-900.0, 0.0, 260.0), FRotator(-4.0, 0.0, 0.0), Spawn));
			}
			Exec(TEXT("r.VSync 0"));
			Exec(TEXT("t.MaxFPS 0"));
			Exec(TEXT("r.AntiAliasingMethod 4"));
			Exec(TEXT("r.ScreenPercentage 67"));
			ApplyCase(CaseIndex);
		}
		break;

	case EPhase::Warmup:
		if (bCompiling)
		{
			PhaseStart = Now;   // shaders still compiling: the numbers would mean nothing yet
		}
		else if (Now - PhaseStart >= WarmupSeconds)
		{
			UE_LOG(LogFSLapBench, Display, TEXT("Case %s: measuring"), Cases()[CaseIndex].Name);
			Phase = EPhase::Measure;
			PhaseStart = Now;
		}
		break;

	case EPhase::Measure:
		FrameMs.Add(FrameTime);
		GpuMs.Add(FPlatformTime::ToMilliseconds(RHIGetGPUFrameCycles()));
		GameMs.Add(FPlatformTime::ToMilliseconds(GGameThreadTime));
		RenderMs.Add(FPlatformTime::ToMilliseconds(GRenderThreadTime));
		if (Now - PhaseStart >= MeasureSeconds)
		{
			FinishCase();
		}
		break;

	case EPhase::Shot:
		if (Now - PhaseStart >= ShotSeconds)
		{
			if (++CaseIndex < Cases().Num())
			{
				ApplyCase(CaseIndex);
			}
			else
			{
				Finish();
			}
		}
		break;

	case EPhase::Done:
		break;
	}
}
