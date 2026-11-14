#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "FSLapSave.generated.h"

USTRUCT()
struct FFSLapSavedLap
{
	GENERATED_BODY()

	UPROPERTY()
	double Time = 0;

	UPROPERTY()
	double Penalty = 0;

	UPROPERTY()
	double Split1 = 0;

	UPROPERTY()
	double Split2 = 0;

	UPROPERTY()
	TArray<double> Ghost;   // t, x, y, h for every sample
};

/** Best laps and their ghosts, one per mode and setup. Kept in Saved\SaveGames\FSLap.sav. */
UCLASS()
class UFSLapSave : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TMap<FString, FFSLapSavedLap> Laps;

	UPROPERTY()
	FString WheelName;   // the steering wheel the setup screen calibrated

	UPROPERTY()
	TArray<float> WheelCal;   // steering, throttle, brake: axis, value at rest, value at full

	UPROPERTY()
	int32 ForceFeedback = 0;   // % of the wheel's strongest force, negative: reversed, 0: off

	UPROPERTY()
	int32 WheelRotation = 0;   // degrees lock to lock as set in the wheel's driver: steering 1:1 with the car; 0: the calibrated lock
};
