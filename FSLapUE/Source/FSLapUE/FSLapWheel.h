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
	// then it looks again every 2 s, so a wheel plugged in later is found. ForceAxis: the axis (0..7) that gets force
	// feedback, -1 for none; a change reopens the controller, since force feedback needs exclusive access.
	bool Read(const FString& Preferred, TArray<float>& Axes, int32 ForceAxis = -1);

	// A constant force on ForceAxis, -1..1 of the controller's strongest (+ pushes towards larger axis values, if the
	// driver follows DirectInput; the setup screen can reverse it). Does nothing without force feedback.
	void SetForce(float Force);

	bool HasForce() const { return Current.Effect != nullptr; }

	static constexpr int32 AxisCount = 8;   // Read gives these axes first, then Buttons buttons as axes (0 or 1)
	static constexpr int32 Buttons = 32;

	FString Name;   // the product name of the controller being read

private:
	struct FOpened { void* Input = nullptr; void* Device = nullptr; void* Effect = nullptr; FString Name; int32 ForceAxis = -1; };
	// Slow (DirectInput's device search takes 0.3 to 0.6 s on the team laptop), so it runs on a worker thread:
	// on the game thread it froze the game every 2 s while no wheel was attached.
	static FOpened Open(const FString& Preferred, void* Window, int32 ForceAxis);
	static void Release(FOpened& Opened);
	void Close();

	FOpened Current;
	TFuture<FOpened> Search;
	double NextTry = 0;
};
