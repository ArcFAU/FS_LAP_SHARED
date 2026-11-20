#include "FSLapSliceTest.h"

#include "Camera/CameraActor.h"
#include "CoreGlobals.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "../FSLapVehiclePawn.h"
#include "AudioMixerBlueprintLibrary.h"
#include "GameFramework/PlayerController.h"
#include "HAL/PlatformTime.h"
#include "Misc/CommandLine.h"
#include "Misc/FileHelper.h"
#include "Misc/Parse.h"
#include "Misc/Paths.h"
#include "RHI.h"
#include "RHIGlobals.h"
#include "RenderTimer.h"
#include "UnrealClient.h"

#if WITH_EDITOR
#include "ShaderCompiler.h"
#endif

namespace FSLapSliceTestLocal
{
	struct FShot { const TCHAR* Name; FVector Offset; };
	const FShot Shots[] = {
		{ TEXT("front"), FVector(620, 0, 90) }, { TEXT("rear"), FVector(-620, 0, 90) },
		{ TEXT("left"), FVector(0, -620, 90) }, { TEXT("right"), FVector(0, 620, 90) },
		{ TEXT("front_left"), FVector(460, -400, 170) }, { TEXT("rear_right"), FVector(-460, 400, 170) },
		{ TEXT("top"), FVector(10, 0, 700) }, { TEXT("overview"), FVector(10, 0, 60000) },
	};
	constexpr double ShotSettle = 4.0, ShotAfter = 2.0;

	// -FSLapMode=keys presses the real keys through the player controller and checks what each one did.
	// Axis steps (Value set) send the analog value every frame, the way a gamepad does.
	struct FKeyStep
	{
		FKey Key;
		double Hold;
		const TCHAR* Name;
		TFunction<bool(const AFSLapVehiclePawn&)> Check;
		float Value = -2;
		const TCHAR* Photo = nullptr;   // a screenshot with this name at the end of the hold
		FVector2f Mouse = FVector2f(-1, -1);   // set: the cursor moves there first (the HUD's 1080-line layout units)
		int32 WheelButton = -1;   // set: a pretend wheel holds this button (an index into the wheel's axes) through the hold
	};
	const TArray<FKeyStep>& KeySteps()
	{
		using Pawn = AFSLapVehiclePawn;
		const auto Menu = [](int32 Row) { return [Row](const Pawn& C) { return C.GetFlow() == EFSLapFlow::Menu && C.GetMenuIndex() == Row; }; };
		const auto FlowIs = [](EFSLapFlow Flow) { return [Flow](const Pawn& C) { return C.GetFlow() == Flow; }; };
		const auto RowHas = [](int32 Row, const TCHAR* Text) { return [Row, Text](const Pawn& C) { return C.GetSetupRowText(Row).Contains(Text); }; };
		// the settings' key row for the menu, from pause (above the whole map's and the reset row) is selected and reads so
		const auto KeyRow = [](const TCHAR* Text) { return [Text](const Pawn& C) { return C.GetFlow() == EFSLapFlow::Settings && C.GetSetupRow() == C.GetSetupRowCount() - 3 && C.GetSetupRowText(C.GetSetupRow()).Contains(Text); }; };
		const auto AssistIs = [](const TCHAR* Text) { return [Text](const Pawn& C) { return C.GetSetupRow() == C.GetSetupRowCount() - 1 && C.GetSetupRowText(C.GetSetupRow()).Contains(Text); }; };
		// the menu's rows from its table, so a new way to drive does not move every step
		const int32 Quit = Pawn::GetMenuRowCount() - 1, Settings = Quit - 1, Setup = Quit - 2, TrackRow = Pawn::GetMenuTrackRow();
		const int32 Arcade = [] { for (int32 r = 0; r < Pawn::GetMenuRowCount(); ++r) { if (!FCString::Strcmp(Pawn::GetMenuName(r), TEXT("ARCADE"))) { return r; } } return -1; }();
		const auto RowY = [](int32 Row) { return 243.0f + Row * 66; };   // the middle of a menu row (FSLapHUD::DrawMenu)
		static const TArray<FKeyStep> Steps = {
			{ EKeys::Down, 0.2, TEXT("menu down"), Menu(1) },
			{ EKeys::Down, 0.2, TEXT("menu down"), Menu(2) },
			{ EKeys::Up, 0.2, TEXT("menu up"), Menu(1) },
			{ EKeys::Up, 0.2, TEXT("menu up"), Menu(0) },
			{ EKeys::Up, 0.2, TEXT("menu wraps to quit"), Menu(Quit) },
			{ EKeys::Up, 0.2, TEXT("menu up to settings"), Menu(Settings) },
			{ EKeys::Enter, 0.5, TEXT("enter opens the settings"), FlowIs(EFSLapFlow::Settings), -2, TEXT("settings.png") },
			{ EKeys::Right, 2.5, TEXT("window mode: from borderless to a window"), [](const Pawn& C) { return C.GetFlow() == EFSLapFlow::Settings && C.GetSetupRowText(0).Contains(TEXT("Window:  window")); } },
			{ EKeys::Down, 0.2, TEXT("down to the resolution"), [](const Pawn& C) { return C.GetFlow() == EFSLapFlow::Settings && C.GetSetupRow() == 1 && C.GetSetupRowText(1).Contains(TEXT("1920 x 1080")); } },
			{ EKeys::Left, 2.0, TEXT("left: a smaller resolution"), [](const Pawn& C) { return C.GetFlow() == EFSLapFlow::Settings && C.GetSetupRowText(1).Contains(TEXT("Resolution:")) && !C.GetSetupRowText(1).Contains(TEXT("1920 x 1080")); } },
			{ EKeys::Left, 2.0, TEXT("left again: still not the full size"), [](const Pawn& C) { return C.GetFlow() == EFSLapFlow::Settings && !C.GetSetupRowText(1).Contains(TEXT("1920 x 1080")); } },
			{ EKeys::Right, 2.0, TEXT("right: one step back up"), [](const Pawn& C) { return C.GetFlow() == EFSLapFlow::Settings && !C.GetSetupRowText(1).Contains(TEXT("1920 x 1080")); } },
			{ EKeys::Right, 2.0, TEXT("right again: the full size is back, so every step moved one entry"), [](const Pawn& C) { return C.GetFlow() == EFSLapFlow::Settings && C.GetSetupRowText(1).Contains(TEXT("1920 x 1080")); } },
			{ EKeys::Up, 0.2, TEXT("up to the window mode"), [](const Pawn& C) { return C.GetFlow() == EFSLapFlow::Settings && C.GetSetupRow() == 0; } },
			{ EKeys::Right, 2.5, TEXT("window mode: full screen again, borderless at the desktop size"), [](const Pawn& C) { return C.GetFlow() == EFSLapFlow::Settings && C.GetSetupRowText(0).Contains(TEXT("full screen (borderless)")) && C.GetSetupRowText(1).Contains(TEXT("1920 x 1080")); } },
			{ EKeys::Up, 0.2, TEXT("settings wrap to the keys' reset row"), [](const Pawn& C) { return C.GetSetupRow() == C.GetSetupRowCount() - 1; } },
			{ EKeys::Up, 0.2, TEXT("up to the whole map's key"), [](const Pawn& C) { return C.GetSetupRow() == C.GetSetupRowCount() - 2 && C.GetSetupRowText(C.GetSetupRow()).EndsWith(TEXT("whole map:  TAB")); } },
			{ EKeys::Up, 0.2, TEXT("up to the key that goes to the menu"), KeyRow(TEXT("menu, from pause:  M")) },
			{ EKeys::Enter, 0.2, TEXT("enter waits for a key"), KeyRow(TEXT("press the new key")) },
			{ EKeys::Up, 0.2, TEXT("an arrow key is refused and moves nothing"), KeyRow(TEXT("M   (UP is a menu or view key)")) },
			{ EKeys::Enter, 0.2, TEXT("enter waits again"), KeyRow(TEXT("press the new key")) },
			{ EKeys::N, 0.2, TEXT("n becomes the key"), KeyRow(TEXT("menu, from pause:  N")) },
			{ EKeys::Enter, 0.2, TEXT("enter waits for a key or a wheel button"), KeyRow(TEXT("press the new key or a wheel button")) },
			{ EKeys::Invalid, 0.2, TEXT("a wheel button becomes the action's button too"), KeyRow(TEXT("menu, from pause:  N   wheel button 5")), -2, nullptr, FVector2f(-1, -1), 12 },
			{ EKeys::Escape, 0.2, TEXT("esc leaves the settings"), Menu(Settings) },
			{ EKeys::Up, 0.2, TEXT("menu up to setup"), Menu(Setup) },
			{ EKeys::Enter, 0.2, TEXT("enter opens setup"), FlowIs(EFSLapFlow::Setup) },
			{ EKeys::Right, 0.2, TEXT("next aero package"), RowHas(0, TEXT("vehicle-params")) },
			{ EKeys::Left, 0.2, TEXT("previous aero package"), RowHas(0, TEXT("LTS vehicle file")) },
			{ EKeys::Down, 0.2, TEXT("setup row down"), [](const Pawn& C) { return C.GetSetupRow() == 1; } },
			{ EKeys::Right, 0.2, TEXT("raise downforce"), RowHas(1, TEXT("5.45")) },
			{ EKeys::R, 0.2, TEXT("r restores the stock car"), RowHas(1, TEXT("5.40")) },
			{ EKeys::Up, 0.2, TEXT("setup row up"), [](const Pawn& C) { return C.GetSetupRow() == 0; } },
			{ EKeys::Up, 0.2, TEXT("setup wraps to driver assist"), AssistIs(TEXT("On")) },
			{ EKeys::Up, 0.2, TEXT("setup row up to force feedback"), [](const Pawn& C) { return C.GetSetupRow() == C.GetSetupRowCount() - 2 && C.GetSetupRowText(C.GetSetupRow()).Contains(TEXT("Force feedback")); } },
			{ EKeys::Up, 0.2, TEXT("setup row up to wheel rotation"), [](const Pawn& C) { return C.GetSetupRow() == C.GetSetupRowCount() - 3 && C.GetSetupRowText(C.GetSetupRow()).Contains(TEXT("Wheel rotation")); } },
			{ EKeys::Up, 0.2, TEXT("setup row up to the steering wheel"), [](const Pawn& C) { return C.GetSetupRow() == C.GetSetupRowCount() - 4 && C.GetSetupRowText(C.GetSetupRow()).Contains(TEXT("Steering wheel")); } },
			{ EKeys::Up, 0.2, TEXT("setup row up to the LTS reference"), [](const Pawn& C) { return C.GetSetupRow() == C.GetSetupRowCount() - 5; } },   // not changed: it would start MATLAB
			{ EKeys::Up, 0.2, TEXT("setup row up to the vehicle model"), [](const Pawn& C) { return C.GetSetupRow() == C.GetSetupRowCount() - 6 && C.GetSetupRowText(C.GetSetupRow()).Contains(TEXT("Vehicle model")); } },
			{ EKeys::Right, 0.5, TEXT("with the ADTM chosen, the load sensitivity row says it does not act (the TMeasy tyres have their own data)"), [](const Pawn& C) { return !C.GetSetupRowText(C.GetSetupRow()).Contains(TEXT("ADTM (team")) || C.GetSetupRowText(12).Contains(TEXT("(not on the ADTM)")); }, -2, TEXT("setup_adtm.png") },
			{ EKeys::Right, 0.2, TEXT("back on FS Lap's model the mark is gone"), [](const Pawn& C) { return C.GetSetupRowText(C.GetSetupRow()).Contains(TEXT("FS Lap")) && !C.GetSetupRowText(12).Contains(TEXT("(not on the ADTM)")) && C.GetSetupRowText(12).Contains(TEXT("Load sensitivity")); } },
			{ EKeys::Down, 0.2, TEXT("setup row down"), [](const Pawn& C) { return C.GetSetupRow() == C.GetSetupRowCount() - 5; } },
			{ EKeys::Down, 0.2, TEXT("setup row down to the steering wheel"), [](const Pawn& C) { return C.GetSetupRow() == C.GetSetupRowCount() - 4; } },
			{ EKeys::Right, 0.5, TEXT("right opens the wheel setup"), FlowIs(EFSLapFlow::Wheel), -2, TEXT("wheel.png") },
			{ EKeys::Enter, 0.2, TEXT("enter with no wheel stays in the wheel setup"), FlowIs(EFSLapFlow::Wheel) },
			{ EKeys::Invalid, 0.3, TEXT("the mouse over BACK"), FlowIs(EFSLapFlow::Wheel), -2, nullptr, FVector2f(754, 68) },
			{ EKeys::LeftMouseButton, 0.2, TEXT("clicking BACK goes back to setup"), FlowIs(EFSLapFlow::Setup) },
			{ EKeys::Down, 0.2, TEXT("setup row down to wheel rotation"), [](const Pawn& C) { return C.GetSetupRow() == C.GetSetupRowCount() - 3; } },   // not changed: it is the owner's saved setting
			{ EKeys::Down, 0.2, TEXT("setup row down to force feedback"), [](const Pawn& C) { return C.GetSetupRow() == C.GetSetupRowCount() - 2; } },   // not changed: it is the owner's saved setting
			{ EKeys::Down, 0.2, TEXT("setup row down to driver assist"), AssistIs(TEXT("On")) },
			{ EKeys::Right, 0.2, TEXT("driver assist off"), AssistIs(TEXT("Off")) },
			{ EKeys::Left, 0.2, TEXT("driver assist on"), AssistIs(TEXT("On")) },
			{ EKeys::Invalid, 0.3, TEXT("the mouse over a setup row selects it"), [](const Pawn& C) { return C.GetSetupRow() == 1; }, -2, nullptr, FVector2f(300, 163) },
			{ EKeys::RightMouseButton, 0.2, TEXT("right-click lowers downforce"), RowHas(1, TEXT("5.35")) },
			{ EKeys::LeftMouseButton, 0.2, TEXT("click raises it again"), RowHas(1, TEXT("5.40")) },
			{ EKeys::Enter, 0.2, TEXT("enter leaves setup"), FlowIs(EFSLapFlow::Menu) },
			{ EKeys::Down, 0.2, TEXT("menu down to settings"), Menu(Settings) },
			{ EKeys::Down, 0.2, TEXT("menu down to quit"), Menu(Quit) },
			{ EKeys::Down, 0.2, TEXT("menu wraps to the top"), Menu(0) },
			{ EKeys::Invalid, 0.3, TEXT("the mouse over arcade selects it"), Menu(Arcade), -2, nullptr, FVector2f(200, RowY(Arcade)) },
			{ EKeys::Invalid, 0.3, TEXT("the mouse over hot lap selects it"), Menu(0), -2, nullptr, FVector2f(200, RowY(0)) },
			{ EKeys::Enter, 0.2, TEXT("enter starts a hot lap"), [](const Pawn& C) { return C.GetFlow() == EFSLapFlow::Countdown && C.GetRaceMode() == HoMoLap::Mode::Sim; } },
			{ EKeys::Escape, 0.2, TEXT("esc pauses"), FlowIs(EFSLapFlow::Paused) },
			{ EKeys::Enter, 0.2, TEXT("enter resumes"), FlowIs(EFSLapFlow::Countdown) },
			{ EKeys::P, 0.2, TEXT("p pauses"), FlowIs(EFSLapFlow::Paused) },
			{ EKeys::R, 0.2, TEXT("r restarts from pause"), FlowIs(EFSLapFlow::Countdown) },
			{ EKeys::Escape, 0.2, TEXT("esc pauses"), FlowIs(EFSLapFlow::Paused) },
			{ EKeys::Invalid, 0.2, TEXT("the wheel button goes to the menu, as n does"), FlowIs(EFSLapFlow::Menu), -2, nullptr, FVector2f(-1, -1), 12 },
			{ EKeys::Enter, 0.2, TEXT("enter starts the hot lap again"), [](const Pawn& C) { return C.GetFlow() == EFSLapFlow::Countdown && C.GetRaceMode() == HoMoLap::Mode::Sim; } },
			{ EKeys::Escape, 0.2, TEXT("esc pauses"), FlowIs(EFSLapFlow::Paused) },
			{ EKeys::M, 0.2, TEXT("m no longer goes to the menu"), FlowIs(EFSLapFlow::Paused) },
			{ EKeys::N, 0.2, TEXT("n goes to the menu"), FlowIs(EFSLapFlow::Menu) },
			{ EKeys::Up, 0.2, TEXT("menu wraps to quit"), Menu(Quit) },
			{ EKeys::Up, 0.2, TEXT("menu up to settings"), Menu(Settings) },
			{ EKeys::Enter, 0.2, TEXT("enter opens the settings"), FlowIs(EFSLapFlow::Settings) },
			{ EKeys::Down, 0.2, TEXT("down to the whole map's key"), [](const Pawn& C) { return C.GetSetupRow() == C.GetSetupRowCount() - 2; } },
			{ EKeys::Down, 0.2, TEXT("down to the keys' reset row"), [](const Pawn& C) { return C.GetSetupRow() == C.GetSetupRowCount() - 1; } },
			{ EKeys::Enter, 0.2, TEXT("the reset row gives m back and takes the wheel button off"), [](const Pawn& C) { return C.GetSetupRowText(C.GetSetupRowCount() - 3).EndsWith(TEXT("menu, from pause:  M")); } },
			{ EKeys::Escape, 0.2, TEXT("esc leaves the settings"), Menu(Settings) },
			{ EKeys::Down, 0.2, TEXT("menu down to quit"), Menu(Quit) },
			{ EKeys::Down, 0.2, TEXT("menu wraps to the top"), Menu(0) },
			{ EKeys::Down, 0.2, TEXT("menu down to SIL"), Menu(1) },
			{ EKeys::Invalid, 0.3, TEXT("the mouse over the track row"), Menu(TrackRow), -2, nullptr, FVector2f(200, RowY(TrackRow)) },
			{ EKeys::Right, 0.3, TEXT("right: the next track"), [](const Pawn& C) { return C.GetChosenMap() == 1 && C.GetMap() == 1; }, -2, TEXT("menu_track.png") },
			{ EKeys::Enter, 0.3, TEXT("enter: the next track again"), [](const Pawn& C) { return C.GetChosenMap() == 2 % C.GetMapCount(); } },
			{ EKeys::Left, 0.3, TEXT("left twice: back to FSG"), [](const Pawn& C) { return C.GetChosenMap() == 1 % C.GetMapCount(); } },
			{ EKeys::Left, 0.3, TEXT("back on FSG"), [](const Pawn& C) { return C.GetChosenMap() == 0 && C.GetMap() == 0; } },
			{ EKeys::Invalid, 0.3, TEXT("the mouse chooses arcade"), Menu(Arcade), -2, nullptr, FVector2f(200, RowY(Arcade)) },
			{ EKeys::Enter, 0.2, TEXT("enter starts arcade"), [](const Pawn& C) { return C.GetFlow() == EFSLapFlow::Countdown && C.GetRaceMode() == HoMoLap::Mode::Arcade; } },
			{ EKeys::C, 0.2, TEXT("c switches to the cockpit"), [](const Pawn& C) { return C.IsOnboard(); } },
			{ EKeys::C, 0.2, TEXT("c switches to the hood"), [](const Pawn& C) { return C.GetCameraView() == 2; } },
			{ EKeys::C, 0.2, TEXT("c switches to the nose"), [](const Pawn& C) { return C.GetCameraView() == 3; } },
			{ EKeys::C, 0.2, TEXT("c switches to chase"), [](const Pawn& C) { return C.GetCameraView() == 0; } },
			{ EKeys::NumPadFour, 0.5, TEXT("numpad 4 held shows the left side"), [](const Pawn& C) { return C.GetHeldView() == 4; }, -2, TEXT("view_left.png") },
			{ EKeys::NumPadSix, 0.5, TEXT("numpad 6 held shows the right side"), [](const Pawn& C) { return C.GetHeldView() == 6; }, -2, TEXT("view_right.png") },
			{ EKeys::NumPadEight, 0.5, TEXT("numpad 8 held shows the front"), [](const Pawn& C) { return C.GetHeldView() == 8; }, -2, TEXT("view_front.png") },
			{ EKeys::Invalid, 0.2, TEXT("letting go goes back to the chase camera"), [](const Pawn& C) { return C.GetHeldView() == 0 && !C.IsOnboard(); } },
			{ EKeys::Invalid, 5.0, TEXT("start lights finish"), FlowIs(EFSLapFlow::Race) },
			{ EKeys::Tab, 0.5, TEXT("tab shows the whole map"), [](const Pawn& C) { return C.IsMapShown(); }, -2, TEXT("map.png") },
			{ EKeys::Tab, 0.2, TEXT("tab closes it"), [](const Pawn& C) { return !C.IsMapShown(); } },
			{ EKeys::W, 1.5, TEXT("w accelerates"), [](const Pawn& C) { return C.GetSimulation()->c.vx > 8; } },
			{ EKeys::A, 0.5, TEXT("a steers left"), [](const Pawn& C) { return C.GetSimulation()->c.st < -0.5; } },
			{ EKeys::D, 0.6, TEXT("d steers right"), [](const Pawn& C) { return C.GetSimulation()->c.st > 0.5; } },
			{ EKeys::S, 0.6, TEXT("s brakes"), [](const Pawn& C) { return C.GetSimulation()->c.brk > 0.5; } },
			{ EKeys::R, 0.2, TEXT("r goes back on track"), [](const Pawn& C) { return FMath::Abs(C.GetSimulation()->c.vx) < 0.5; } },
			{ EKeys::A, 1.0, TEXT("a alone leaves the stopped car stopped"), [](const Pawn& C) { const HoMoLap::Car& Car = C.GetSimulation()->c; return FMath::Abs(Car.vx) < 0.05 && FMath::Abs(Car.vy) < 0.05; } },
			{ EKeys::W, 2.5, TEXT("w accelerates again"), [](const Pawn& C) { return C.GetSimulation()->c.vx > 8; } },   // 8, not 15: flat out from where R puts the car it can reach the grass in the next bend
			{ EKeys::SpaceBar, 0.9, TEXT("space is the handbrake"), [](const Pawn& C) { return C.GetSimulation()->c.slipR; }, -2, TEXT("slide.png") },
			{ EKeys::R, 0.2, TEXT("r goes back on track"), [](const Pawn& C) { return FMath::Abs(C.GetSimulation()->c.vx) < 0.5; } },
			{ EKeys::Gamepad_RightTriggerAxis, 1.0, TEXT("right trigger is analog throttle"), [](const Pawn& C) { const double T = C.GetSimulation()->c.thr; return T > 0.5 && T < 0.7; }, 0.6f },
			{ EKeys::Gamepad_LeftTriggerAxis, 1.0, TEXT("left trigger is analog brake"), [](const Pawn& C) { const double B = C.GetSimulation()->c.brk; return B > 0.3 && B < 0.5; }, 0.4f },
			{ EKeys::Gamepad_LeftX, 0.6, TEXT("left stick is analog steering"), [](const Pawn& C) { const double S = C.GetSimulation()->c.st; return S > 0.2 && S < 0.5; }, 0.5f },
		};
		return Steps;
	}
}

bool UFSLapSliceTest::ShouldCreateSubsystem(UObject* Outer) const
{
	return Super::ShouldCreateSubsystem(Outer) && FParse::Param(FCommandLine::Get(), TEXT("FSLapSliceTest"));
}

bool UFSLapSliceTest::DoesSupportWorldType(const EWorldType::Type WorldType) const
{
	return WorldType == EWorldType::Game;
}

TStatId UFSLapSliceTest::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UFSLapSliceTest, STATGROUP_Tickables);
}

FString UFSLapSliceTest::OutputDir() const
{
	return FPaths::ConvertRelativePathToFull(FPaths::ProjectSavedDir() / TEXT("SliceTest") / ModeName);
}

void UFSLapSliceTest::Finish(AFSLapVehiclePawn* Car)
{
	TArray<double> Sorted = FrameMs;
	Sorted.Sort();
	double Sum = 0.0, GpuSum = 0.0;
	for (double Value : FrameMs) { Sum += Value; }
	for (double Value : GpuMs) { GpuSum += Value; }
	const double Avg = FrameMs.Num() ? Sum / FrameMs.Num() : 0.0;
	FString Text = FString::Printf(TEXT("adapter=%s\nmode=%s\nframes=%d\nfps=%.1f\nframe_ms=%.2f\nframe_ms_p95=%.2f\ngpu_ms=%.2f\n"), *GRHIAdapterName, *ModeName, FrameMs.Num(), Avg > 0.0 ? 1000.0 / Avg : 0.0, Avg,
		Sorted.Num() ? Sorted[Sorted.Num() * 95 / 100] : 0.0, GpuMs.Num() ? GpuSum / GpuMs.Num() : 0.0);
	// hitches: a frame over 100 ms is a visible jump (the camera and the exposure catch up at once)
	Text += FString::Printf(TEXT("frame_ms_max=%.1f\nframes_over_100ms=%d\n"), Sorted.Num() ? Sorted.Last() : 0.0, FrameMs.FilterByPredicate([](double Ms) { return Ms > 100.0; }).Num());
	for (const HoMoLap::LapResult& Lap : Car->GetSimulation()->laps)
	{
		Text += FString::Printf(TEXT("lap %d: %.3f s (S1 %.3f, S2 %.3f, penalty %.0f, cones %d, off %d, valid %d)\n"), Lap.n, Lap.time, Lap.split1, Lap.split2, Lap.pen, Lap.cones, Lap.offCourse, Lap.valid ? 1 : 0);
	}
	Text += FString::Printf(TEXT("best=%.3f\ntraffic=%d\n"), Car->GetBestLap(), Car->GetTrafficCount());
	Text += FString::Printf(TEXT("reference=%s %.3f\nreference_status=%s\nlap_log=%s\n"), *Car->GetLtsLabel(), Car->GetSimulation()->T.ltsLap, *Car->Lts.Status(), *Car->Log.LastWritten());
	Text += FString::Printf(TEXT("vehicle_model=%s\n"), *Car->KernelName(Car->GetRaceMode()));
	if (Car->IsSil())
	{
		Text += FString::Printf(TEXT("sil=%s\n"), *Car->GetSilStatus());
	}
	Text += FString::Printf(TEXT("replay_keys=%d\n"), bReplayKeys ? 1 : 0);
	FFileHelper::SaveStringToFile(Text, *(OutputDir() / TEXT("results.txt")));
	Phase = EPhase::Done;
	RequestEngineExit(TEXT("FSLap slice test finished"));
}

void UFSLapSliceTest::TickKeys(APlayerController* Player, AFSLapVehiclePawn* Car, double Now)
{
	using namespace FSLapSliceTestLocal;
	const TArray<FKeyStep>& Steps = KeySteps();
	if (KeyIndex >= Steps.Num())
	{
		KeyReport += FString::Printf(TEXT("%d steps, %d failed\nvehicle_model=%s\n"), Steps.Num(), KeyFailures, *Car->KernelName(Car->GetRaceMode()));
		FFileHelper::SaveStringToFile(KeyReport, *(OutputDir() / TEXT("results.txt")));
		Phase = EPhase::Done;
		RequestEngineExit(TEXT("FSLap key test finished"));
		return;
	}
	const FKeyStep& Step = Steps[KeyIndex];
	const bool bAxis = Step.Value > -2;
	if (!bKeyDown)
	{
		if (Step.Mouse.X >= 0)
		{
			int32 W, H;
			Player->GetViewportSize(W, H);
			Player->SetMouseLocation(FMath::RoundToInt32(Step.Mouse.X * H / 1080), FMath::RoundToInt32(Step.Mouse.Y * H / 1080));
		}
		if (Step.Key.IsValid() && !bAxis)
		{
			Player->InputKey(FInputKeyEventArgs::CreateSimulated(Step.Key, IE_Pressed, 1.0f));
		}
		Car->TestWheelButton = Step.WheelButton;
		bKeyDown = true;
		KeyTime = Now;
		KeyFrames = 0;
		return;
	}
	++KeyFrames;
	if (bAxis)
	{
		Player->InputKey(FInputKeyEventArgs::CreateSimulated(Step.Key, IE_Axis, Step.Value));
	}
	if (Now - KeyTime >= Step.Hold)
	{
		const bool bOk = Step.Check(*Car);
		KeyFailures += bOk ? 0 : 1;
		if (Step.Photo)
		{
			FScreenshotRequest::RequestScreenshot(OutputDir() / Step.Photo, false, false);
		}
		KeyReport += FString::Printf(TEXT("%s  %s  (%.0f fps, vx %.1f m/s)\n"), bOk ? TEXT("PASS") : TEXT("FAIL"), Step.Name, KeyFrames / FMath::Max(Now - KeyTime, 1e-3), Car->GetSimulation()->c.vx);
		if (bAxis)
		{
			Player->InputKey(FInputKeyEventArgs::CreateSimulated(Step.Key, IE_Axis, 0.0f));
		}
		else if (Step.Key.IsValid())
		{
			Player->InputKey(FInputKeyEventArgs::CreateSimulated(Step.Key, IE_Released, 0.0f));
		}
		Car->TestWheelButton = -1;
		bKeyDown = false;
		++KeyIndex;
	}
}

void UFSLapSliceTest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	using namespace FSLapSliceTestLocal;
	const double Now = FPlatformTime::Seconds();
	const double FrameTime = LastTime > 0.0 ? (Now - LastTime) * 1000.0 : 0.0;
	LastTime = Now;
	if (PhaseStart == 0.0)
	{
		PhaseStart = Now;
		FParse::Value(FCommandLine::Get(), TEXT("FSLapMode="), ModeName);
	}
	APlayerController* Player = GetWorld()->GetFirstPlayerController();
	AFSLapVehiclePawn* Car = Player ? Cast<AFSLapVehiclePawn>(Player->GetPawn()) : nullptr;
	if (!Car || !Car->GetSimulation())
	{
		return;
	}
	// -FSLapMode=lts: the real LTS computes the reference for the stock setup first (single track), then the hot lap runs against it
	if (ModeName == TEXT("lts") && Phase == EPhase::Warmup)
	{
		if (LtsAskedAt == 0.0)
		{
			LtsAskedAt = Now;
			Car->LtsModel = 2;
			Car->RequestLts();
		}
		if (Car->Lts.IsBusy() && Now - LtsAskedAt < 400.0)
		{
			PhaseStart = Now;   // hold the warm-up until the LTS lap is there (or failed, or 400 s passed)
		}
	}
	// -FSLapMode=adtm: the hot lap with the team's ADTM as the vehicle model; -FSLapAdtm: any mode with it
	if ((ModeName == TEXT("adtm") || FParse::Param(FCommandLine::Get(), TEXT("FSLapAdtm"))) && Phase == EPhase::Warmup)
	{
		Car->bAdtm = true;
	}
	const HoMoLap::Mode Mode = ModeName == TEXT("arcade") ? HoMoLap::Mode::Arcade : (ModeName == TEXT("free") ? HoMoLap::Mode::Free : (ModeName == TEXT("city") ? HoMoLap::Mode::City : HoMoLap::Mode::Sim));
	bool bCompiling = false;
#if WITH_EDITOR
	bCompiling = GShaderCompilingManager && GShaderCompilingManager->IsCompiling();
#endif

	switch (Phase)
	{
	case EPhase::Start:
		if (Now - PhaseStart >= 5.0)
		{
			for (const TCHAR* Command : { TEXT("r.VSync 0"), TEXT("t.MaxFPS 0"), TEXT("r.ScreenPercentage 67"), TEXT("sg.ViewDistanceQuality 1"), TEXT("sg.AntiAliasingQuality 1"), TEXT("sg.ShadowQuality 1"),
				TEXT("sg.GlobalIlluminationQuality 1"), TEXT("sg.ReflectionQuality 1"), TEXT("sg.PostProcessQuality 1"), TEXT("sg.TextureQuality 1"), TEXT("sg.EffectsQuality 1"), TEXT("sg.FoliageQuality 1"), TEXT("sg.ShadingQuality 1") })
			{
				GEngine->Exec(GetWorld(), Command);
			}
			Phase = EPhase::Warmup;
			PhaseStart = Now;
		}
		break;

	case EPhase::Warmup:
		if (bCompiling)
		{
			PhaseStart = Now;   // shaders still compiling: the numbers would mean nothing yet
		}
		else if (Now - PhaseStart >= 15.0 && ModeName == TEXT("keys"))
		{
			Phase = EPhase::Keys;
		}
		else if (Now - PhaseStart >= 15.0)
		{
			static const TCHAR* const Rows[] = { TEXT("sim"), TEXT("sil"), TEXT("arcade"), TEXT("city"), TEXT("free") };   // menu order; another track: the TRACK row after them
			const int32 TrackMap = ModeName == TEXT("czech") ? Car->FindMap(TEXT("fscz")) : (ModeName == TEXT("france") ? Car->FindMap(TEXT("fsf")) : 0);
			if (TrackMap != Car->GetChosenMap())
			{
				Car->ChooseMap(TrackMap - Car->GetChosenMap());
			}
			for (int32 Row = 0; Row < UE_ARRAY_COUNT(Rows) && ModeName != Rows[Row]; ++Row)
			{
				Car->MenuDown();   // the menu photo shows this mode's note
			}
			FScreenshotRequest::RequestScreenshot(OutputDir() / TEXT("menu.png"), false, false);
			Phase = EPhase::MenuShot;
			PhaseStart = Now;
		}
		break;

	case EPhase::MenuShot:
		if (Now - PhaseStart >= 2.0)
		{
			Car->StartRace(Mode, Car->GetChosenMap(), ModeName == TEXT("sil"));
			Phase = EPhase::Countdown;
			PhaseStart = Now;
			bPhaseShot = false;
		}
		break;

	case EPhase::Countdown:
		if (Mode == HoMoLap::Mode::Free)
		{
			// flat out down the strip, then the handbrake: the paddock is paved everywhere, so the smoke and skid marks show
			const double Elapsed = Now - PhaseStart;
			if (FreeStep == 0)
			{
				Player->InputKey(FInputKeyEventArgs::CreateSimulated(EKeys::W, IE_Pressed, 1.0f));
				FreeStep = 1;
			}
			else if (FreeStep == 1 && Elapsed >= 3.0)
			{
				Player->InputKey(FInputKeyEventArgs::CreateSimulated(EKeys::SpaceBar, IE_Pressed, 1.0f));
				FreeStep = 2;
			}
			else if (FreeStep == 2 && Elapsed >= 3.7)
			{
				FScreenshotRequest::RequestScreenshot(OutputDir() / TEXT("slide.png"), false, false);
				FreeStep = 3;
			}
			else if (FreeStep == 3 && Elapsed >= 4.3)
			{
				Player->InputKey(FInputKeyEventArgs::CreateSimulated(EKeys::W, IE_Released, 0.0f));
				Player->InputKey(FInputKeyEventArgs::CreateSimulated(EKeys::SpaceBar, IE_Released, 0.0f));
				Car->bFrozen = true;
				Phase = EPhase::Shots;
				PhaseStart = Now;
				ShotIndex = -1;
				bShotTaken = true;
			}
		}
		else if (Car->GetFlow() == EFSLapFlow::Race)
		{
			Car->bAutopilot = true;
			bRecordAudio = FParse::Param(FCommandLine::Get(), TEXT("FSLapRecordAudio"));
			if (bRecordAudio)
			{
				UAudioMixerBlueprintLibrary::StartRecordingOutput(GetWorld(), 46.0f);
			}
			Phase = EPhase::Lap;
			PhaseStart = Now;
			bPhaseShot = false;
		}
		else if (!bPhaseShot && Now - PhaseStart >= 2.0)
		{
			FScreenshotRequest::RequestScreenshot(OutputDir() / TEXT("countdown.png"), false, false);
			bPhaseShot = true;
		}
		break;

	case EPhase::Lap:
		if (Car->GetSimulation()->laps.empty())
		{
			FrameMs.Add(FrameTime);
			GpuMs.Add(FPlatformTime::ToMilliseconds(RHIGetGPUFrameCycles()));
			if (bRecordAudio)
			{
				AudioTrack.Add(FString::Printf(TEXT("%.3f,%.1f"), Now - PhaseStart, Car->GetMotorTone()));
				if (Now - PhaseStart >= 45.0)
				{
					UAudioMixerBlueprintLibrary::StopRecordingOutput(GetWorld(), EAudioRecordingExportType::WavFile, TEXT("audio"), FPaths::ConvertRelativePathToFull(OutputDir()));
					FFileHelper::SaveStringArrayToFile(AudioTrack, *(OutputDir() / TEXT("audio_track.csv")));
					bRecordAudio = false;
				}
			}
			if (!bPhaseShot && Now - PhaseStart >= 30.0)
			{
				FScreenshotRequest::RequestScreenshot(OutputDir() / TEXT("chase.png"), false, false);
				bPhaseShot = true;
			}
			if (OnboardStep == 0 && Now - PhaseStart >= 40.0)
			{
				Car->CycleCamera();
				OnboardStep = 1;
			}
			else if (OnboardStep == 1 && Now - PhaseStart >= 43.0)
			{
				FScreenshotRequest::RequestScreenshot(OutputDir() / TEXT("onboard.png"), false, false);
				OnboardStep = 2;
			}
			else if (OnboardStep >= 2 && OnboardStep <= 6 && Now - PhaseStart >= 46.0 + (OnboardStep - 2) * 2.5)
			{
				// 2: to the hood, 3: its photo, 4: to the nose, 5: its photo, 6: back to chase
				if (OnboardStep == 3 || OnboardStep == 5)
				{
					FScreenshotRequest::RequestScreenshot(OutputDir() / (OnboardStep == 3 ? TEXT("hood.png") : TEXT("nose.png")), false, false);
				}
				else
				{
					Car->CycleCamera();
				}
				++OnboardStep;
			}
		}
		else if (Car->GetSimulation()->CurrentLapTime() > 20.0 && (FMath::Abs(Car->GetSimulation()->c.delta) > 0.12 || Car->GetSimulation()->CurrentLapTime() > 45.0))   // in a bend if there is one sharp enough, so the steered wheels show, with the ghosts well apart
		{
			const HoMoLap::Simulation& Sim = *Car->GetSimulation();
			int32 NearestCone = 0;
			double BestDistance = TNumericLimits<double>::Max();
			for (int32 i = 0; i < static_cast<int32>(Sim.T.cones.size()); ++i)
			{
				const double Distance = FMath::Square(Sim.T.cones[i].x - Sim.c.x) + FMath::Square(Sim.T.cones[i].y - Sim.c.y);
				if (!Sim.T.cones[i].freeOnly && Distance < BestDistance)
				{
					BestDistance = Distance;
					NearestCone = i;
				}
			}
			if (!Sim.T.cones.empty())
			{
				Car->KnockDownConeForTest(NearestCone);
			}
			Player->InputKey(FInputKeyEventArgs::CreateSimulated(EKeys::Escape, IE_Pressed, 1.0f));
			Player->InputKey(FInputKeyEventArgs::CreateSimulated(EKeys::Escape, IE_Released, 0.0f));
			Phase = EPhase::Replay;
			PhaseStart = Now;
		}
		break;

	case EPhase::Replay:   // through the real keys: Esc has paused, then B replays, Esc leaves it, Enter resumes
	{
		const double Elapsed = Now - PhaseStart;
		const auto Press = [Player](const FKey& Key)
		{
			Player->InputKey(FInputKeyEventArgs::CreateSimulated(Key, IE_Pressed, 1.0f));
			Player->InputKey(FInputKeyEventArgs::CreateSimulated(Key, IE_Released, 0.0f));
		};
		if (ReplayStep == 0 && Elapsed >= 0.5)
		{
			bReplayKeys = Car->GetFlow() == EFSLapFlow::Paused;
			Press(EKeys::B);
			ReplayStep = 1;
		}
		else if (ReplayStep == 1 && Elapsed >= 6.0)
		{
			bReplayKeys &= Car->GetFlow() == EFSLapFlow::Replay;
			FScreenshotRequest::RequestScreenshot(OutputDir() / TEXT("replay.png"), false, false);
			ReplayStep = 2;
		}
		else if (ReplayStep == 2 && Elapsed >= 7.5)
		{
			Press(EKeys::Escape);
			ReplayStep = 3;
		}
		else if (ReplayStep == 3 && Elapsed >= 8.0)
		{
			bReplayKeys &= Car->GetFlow() == EFSLapFlow::Paused;
			Press(EKeys::Enter);
			ReplayStep = 4;
		}
		else if (ReplayStep == 4 && Elapsed >= 8.5)
		{
			bReplayKeys &= Car->GetFlow() == EFSLapFlow::Race;
			Car->bFrozen = true;
			Phase = EPhase::Shots;
			PhaseStart = Now;
			ShotIndex = -1;   // the first pass of Shots places the first camera
			bShotTaken = true;
		}
		break;
	}

	case EPhase::Shots:
		if (bShotTaken && (ShotIndex < 0 || Now - PhaseStart >= ShotAfter))
		{
			// in the cities, more photos after the car's: the first traffic car, the highest bridge from beside, from a
			// car on the ground beneath it and from 700 m (beyond the road tiles' culling), then the whole map
			struct FExtra { const TCHAR* Name; FVector Position, Target; bool bMap; };
			TArray<FExtra> Extras;
			FVector Traffic;
			if (Car->GetTrafficCar(Traffic))
			{
				Extras.Add({ TEXT("traffic"), Traffic + FVector(-1400, -900, 900), Traffic, false });
				const HoMoLap::Track& T = Car->GetSimulation()->T;
				const HoMoLap::Deck* High = nullptr;
				double Clear = 0;
				for (const HoMoLap::Deck& D : T.decks)
				{
					const double Mx = (D.x0 + D.x1) / 2, My = (D.y0 + D.y1) / 2, Above = (D.z0 + D.z1) / 2 - T.HeightAt(Mx, My, -1e4);
					if (Above > Clear) { Clear = Above; High = &D; }
				}
				if (High)
				{
					const FVector A(High->x0, High->y0, High->z0), B(High->x1, High->y1, High->z1), Mid = (A + B) * 50;   // cm
					const FVector Across = FVector(-(B.Y - A.Y), B.X - A.X, 0).GetSafeNormal();
					const double Ground = T.HeightAt(Mid.X / 100, Mid.Y / 100, -1e4) * 100;
					Extras.Add({ TEXT("bridge_side"), Mid + Across * 4000 + FVector(0, 0, 600), Mid, false });
					Extras.Add({ TEXT("bridge_under"), FVector(Mid.X, Mid.Y, Ground) + Across * 2500 + FVector(0, 0, 150), FVector(Mid.X, Mid.Y, Ground + 300), false });
					Extras.Add({ TEXT("bridge_far"), Mid + Across * 50000 + FVector(0, 0, 40000), Mid, false });
				}
				Extras.Add({ TEXT("map"), FVector::ZeroVector, FVector::ZeroVector, true });
			}
			const int32 Count = UE_ARRAY_COUNT(Shots);
			if (++ShotIndex >= Count + Extras.Num())
			{
				Car->SetMapShown(false);
				Finish(Car);
				break;
			}
			ShotName = ShotIndex < Count ? Shots[ShotIndex].Name : Extras[ShotIndex - Count].Name;
			if (ShotIndex >= Count && Extras[ShotIndex - Count].bMap)
			{
				Car->SetMapShown(true);
				bShotTaken = false;
				PhaseStart = Now;
				break;
			}
			const FVector Target = ShotIndex < Count ? Car->GetActorTransform().TransformPosition(FVector(0, 0, 50)) : Extras[ShotIndex - Count].Target;
			const FVector Position = ShotIndex < Count ? Car->GetActorTransform().TransformPosition(Shots[ShotIndex].Offset) : Extras[ShotIndex - Count].Position;
			FActorSpawnParameters Spawn;
			Spawn.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			Player->SetViewTarget(GetWorld()->SpawnActor<ACameraActor>(Position, (Target - Position).Rotation(), Spawn));
			bShotTaken = false;
			PhaseStart = Now;
		}
		else if (!bShotTaken && Now - PhaseStart >= ShotSettle)
		{
			FScreenshotRequest::RequestScreenshot(OutputDir() / (ShotName + TEXT(".png")), false, false);
			bShotTaken = true;
			PhaseStart = Now;
		}
		break;

	case EPhase::Keys:
		TickKeys(Player, Car, Now);
		break;

	case EPhase::Done:
		break;
	}
}
