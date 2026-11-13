#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "FSLapSliceTest.generated.h"

/**
 * Acceptance run in the real game. It does nothing unless the game is started with -FSLapSliceTest (tools\slice_test.ps1 does that).
 * It starts the chosen mode (-FSLapMode=sim|arcade|free) through the normal game flow at Medium settings. In sim and arcade the AI
 * drives a lap while frame times are recorded and goes on into the next lap, so the ghosts show. Then the car is photographed from
 * all sides. Writes Saved\SliceTest\<mode>\results.txt and PNGs, then quits.
 */
UCLASS()
class UFSLapSliceTest : public UTickableWorldSubsystem
{
	GENERATED_BODY()

public:
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual bool DoesSupportWorldType(const EWorldType::Type WorldType) const override;
	virtual void Tick(float DeltaTime) override;
	virtual TStatId GetStatId() const override;

private:
	bool bRecordAudio = false;
	TArray<FString> AudioTrack;
	enum class EPhase : uint8 { Start, Warmup, MenuShot, Countdown, Lap, Replay, Shots, Keys, Done };

	void Finish(class AFSLapVehiclePawn* Car);
	void TickKeys(class APlayerController* Player, class AFSLapVehiclePawn* Car, double Now);
	FString OutputDir() const;

	EPhase Phase = EPhase::Start;
	FString ModeName = TEXT("sim");
	double PhaseStart = 0.0;
	double LastTime = 0.0;
	bool bPhaseShot = false;
	int32 OnboardStep = 0;
	int32 FreeStep = 0;
	int32 ReplayStep = 0;
	bool bReplayKeys = false;   // pause, B, Esc and Enter did what they should
	int32 KeyIndex = 0;
	bool bKeyDown = false;
	double KeyTime = 0.0;
	int32 KeyFrames = 0;   // frames during the current step: a step that fails at a low frame rate says so
	FString KeyReport;
	int32 KeyFailures = 0;
	int32 ShotIndex = 0;
	bool bShotTaken = false;
	TArray<double> FrameMs;
	TArray<double> GpuMs;
};
