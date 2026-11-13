#pragma once

#include "Async/Future.h"
#include "CoreMinimal.h"

/**
 * A steering wheel or other DirectInput game controller (e.g. Moza R3), which Unreal's own input does not see:
 * it only reads XInput pads. Reads up to 8 axes, each -1..1. Which axis is steering, throttle or brake is not
 * known here; the wheel setup screen records that.
 */
class FFSLapWheel
{
public:
	~FFSLapWheel();

	// Preferred: the product name to pick if several controllers are attached. False while no controller is found;
	// then it looks again every 2 s, so a wheel plugged in later is found.
	bool Read(const FString& Preferred, TArray<float>& Axes);

	FString Name;   // the product name of the controller being read

private:
	struct FOpened { void* Input = nullptr; void* Device = nullptr; FString Name; };
	// Slow (DirectInput's device search takes 0.3 to 0.6 s on the team laptop), so it runs on a worker thread:
	// on the game thread it froze the game every 2 s while no wheel was attached.
	static FOpened Open(const FString& Preferred, void* Window);
	static void Release(FOpened& Opened);
	void Close();

	FOpened Current;
	TFuture<FOpened> Search;
	double NextTry = 0;
};
