#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "FSLapGpuBench.generated.h"

/**
 * Stage 0 GPU benchmark. It does nothing unless the game is started with -FSLapGpuBench
 * (tools\gpu_test\run.ps1 does that). It steps through lighting setups, from plain lighting
 * to Lumen, measures frame and GPU time for each, saves a screenshot, writes
 * Saved\GpuBench\results.csv and quits.
 */
UCLASS()
class UFSLapGpuBench : public UTickableWorldSubsystem
{
	GENERATED_BODY()

public:
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual bool DoesSupportWorldType(const EWorldType::Type WorldType) const override;
	virtual void Tick(float DeltaTime) override;
	virtual TStatId GetStatId() const override;

private:
	enum class EPhase : uint8 { Start, Warmup, Measure, Shot, Done };

	void ApplyCase(int32 Index);
	void FinishCase();
	void Finish();
	void Exec(const FString& Command);
	FString OutputDir() const;

	EPhase Phase = EPhase::Start;
	int32 CaseIndex = 0;
	double BenchStart = 0.0;
	double PhaseStart = 0.0;
	double LastTime = 0.0;

	TArray<double> FrameMs;
	TArray<double> GpuMs;
	TArray<double> GameMs;
	TArray<double> RenderMs;
	TArray<FString> Rows;
};
