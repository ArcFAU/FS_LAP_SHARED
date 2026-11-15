// The settings screen of AFSLapVehiclePawn: display (the engine's game user settings, which it saves itself),
// master volume and the keys of the driving actions (both in the save game).
#include "FSLapVehiclePawn.h"

#include "FSLapSave.h"
#include "GameFramework/GameUserSettings.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/PlayerInput.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Misc/App.h"
#include "Misc/CommandLine.h"
#include "Misc/Paths.h"

namespace
{
	const TCHAR* const SaveSlot = TEXT("FSLap");   // as in FSLapVehiclePawn.cpp

	// The driving actions whose keyboard key can be changed, in EFSLapKey order: the mapping in Config\DefaultInput.ini
	// (axis with its scale, or action with scale 0) and its key there. The arrow keys, the gamepad and the menu keys stay.
	struct FKeyItem { const TCHAR* Label; FName Mapping; float Scale; FKey Default; };
	const FKeyItem& KeyItem(int32 Item)
	{
		static const FKeyItem Items[EFSLapKey::Count] = {
			{ TEXT("throttle"), TEXT("Throttle"), 1, EKeys::W },
			{ TEXT("brake"), TEXT("Brake"), 1, EKeys::S },
			{ TEXT("steer left"), TEXT("Steer"), -1, EKeys::A },
			{ TEXT("steer right"), TEXT("Steer"), 1, EKeys::D },
			{ TEXT("handbrake"), TEXT("Handbrake"), 0, EKeys::SpaceBar },
			{ TEXT("back on track / restart"), TEXT("ResetCar"), 0, EKeys::R },
			{ TEXT("camera"), TEXT("CameraCycle"), 0, EKeys::C },
			{ TEXT("pause"), TEXT("Pause"), 0, EKeys::P },
			{ TEXT("replay of the best lap"), TEXT("Replay"), 0, EKeys::B },
			{ TEXT("menu, from pause"), TEXT("BackToMenu"), 0, EKeys::M },
		};
		return Items[Item];
	}

	enum ERow { Window, Resolution, Quality, VSync, FrameLimit, Volume, ViewAngle, EyeHeight, EyeForward, FirstKey, ResetKeys = FirstKey + EFSLapKey::Count, RowCount };
	const int32 FrameLimits[] = { 0, 30, 60, 90, 120, 144, 165, 240 };

	// Full screen is always borderless, at the desktop's size. True if a setting was changed.
	// - Exclusive full screen is not used: when another window takes the focus (a click on a second monitor while the
	//   game loads is enough), the game loses the mode and takes it back in an endless loop on one frame (seen 2026-11-14).
	// - A full-screen mode at a window's size (1920 x 1009) left a 320 x 200 picture, and the next change crashed.
	bool FitResolution(UGameUserSettings* Display)
	{
		bool bChanged = false;
		if (Display->GetFullscreenMode() == EWindowMode::Fullscreen)
		{
			Display->SetFullscreenMode(EWindowMode::WindowedFullscreen);
			bChanged = true;
		}
		if (Display->GetFullscreenMode() == EWindowMode::WindowedFullscreen && Display->GetScreenResolution() != Display->GetDesktopResolution())
		{
			Display->SetScreenResolution(Display->GetDesktopResolution());
			bChanged = true;
		}
		return bChanged;
	}

	// the saved keys only for a player: a test run presses the default keys and must not change the owner's
	bool IsTestRun() { return FCString::Stristr(FCommandLine::Get(), TEXT("FSLapMode=")) != nullptr; }
}

int32 AFSLapVehiclePawn::GetSettingsRowCount() const
{
	return RowCount;
}

FString AFSLapVehiclePawn::GetKeyName(int32 Item) const
{
	const FKey Key = Keys.IsValidIndex(Item) && !Keys[Item].IsNone() ? FKey(Keys[Item]) : KeyItem(Item).Default;
	return Key.GetDisplayName(/*bLongDisplayName=*/false).ToString().ToUpper();
}

FString AFSLapVehiclePawn::GetSettingsRowText(int32 Row) const
{
	const UGameUserSettings* Display = GEngine->GetGameUserSettings();
	if (Row >= FirstKey && Row < ResetKeys)
	{
		const int32 Item = Row - FirstKey;
		return FString::Printf(TEXT("Key, %s:  %s"), KeyItem(Item).Label, Item == CaptureItem ? TEXT("press the new key (ESC keeps the old one)")
			: *(GetKeyName(Item) + (Row == SettingsRow && !KeyMessage.IsEmpty() ? TEXT("   (") + KeyMessage + TEXT(")") : FString())));
	}
	switch (Row)
	{
	case Window:
		return FString::Printf(TEXT("Window:  %s"), Display->GetFullscreenMode() == EWindowMode::Windowed ? TEXT("window") : TEXT("full screen (borderless)"));
	case Resolution:
		return FString::Printf(TEXT("Resolution:  %d x %d%s"), Display->GetScreenResolution().X, Display->GetScreenResolution().Y,
			Display->GetFullscreenMode() == EWindowMode::WindowedFullscreen ? TEXT("   (borderless uses the desktop's)") : TEXT(""));
	case Quality:
	{
		static const TCHAR* const Names[] = { TEXT("low"), TEXT("medium"), TEXT("high"), TEXT("epic"), TEXT("cinematic") };
		const int32 Level = Display->GetOverallScalabilityLevel();   // -1: the groups differ
		return FString::Printf(TEXT("Graphics quality:  %s"), Level >= 0 && Level < UE_ARRAY_COUNT(Names) ? Names[Level] : TEXT("custom"));
	}
	case VSync:
		return FString::Printf(TEXT("VSync:  %s"), Display->IsVSyncEnabled() ? TEXT("on") : TEXT("off"));
	case FrameLimit:
		return Display->GetFrameRateLimit() <= 0 ? FString(TEXT("Frame rate limit:  off")) : FString::Printf(TEXT("Frame rate limit:  %.0f fps"), Display->GetFrameRateLimit());
	case Volume:
		return FString::Printf(TEXT("Master volume:  %d %%"), SaveData ? SaveData->MasterVolume : 100);
	case ViewAngle:
	{
		const int32 Fov = SaveData ? SaveData->OnboardFov : 0;
		return Fov == 0 ? FString(TEXT("Onboard view angle:  default (60-63\u00B0 with speed)")) : FString::Printf(TEXT("Onboard view angle:  %d\u00B0 vertical, fixed"), Fov);
	}
	case EyeHeight:
		return FString::Printf(TEXT("Cockpit view, eye height:  %+d mm"), SaveData ? SaveData->EyeHeight : 0);
	case EyeForward:
		return FString::Printf(TEXT("Cockpit view, eye forward:  %+d mm"), SaveData ? SaveData->EyeForward : 0);
	default:
		return TEXT("Keys back to their defaults (ENTER)");
	}
}

void AFSLapVehiclePawn::ChangeSettings(int32 Direction)
{
	KeyMessage.Empty();
	UGameUserSettings* Display = GEngine->GetGameUserSettings();
	if (SettingsRow >= FirstKey && SettingsRow < ResetKeys)
	{
		CaptureItem = SettingsRow - FirstKey;   // UpdateKeyCapture takes the next key
		bCaptureFresh = true;
		return;
	}
	switch (SettingsRow)
	{
	case Window:
		Display->SetFullscreenMode(Display->GetFullscreenMode() == EWindowMode::Windowed ? EWindowMode::WindowedFullscreen : EWindowMode::Windowed);
		break;
	case Resolution:
	{
		TArray<FIntPoint> Sizes;
		UKismetSystemLibrary::GetSupportedFullscreenResolutions(Sizes);
		if (Sizes.IsEmpty())
		{
			return;
		}
		// one step from the current size in the display's list (ordered by width, so not by pixel count);
		// from a size that is not in the list, such as a resized window's, the first one at least as wide
		int32 Index = Sizes.IndexOfByKey(Display->GetScreenResolution());
		if (Index < 0)
		{
			Index = Sizes.IndexOfByPredicate([Display](const FIntPoint& Size) { return Size.X >= Display->GetScreenResolution().X; });
			Index = (Index < 0 ? Sizes.Num() : Index) - (Direction > 0 ? 1 : 0);
		}
		Index = FMath::Clamp(Index + Direction, 0, Sizes.Num() - 1);
		Display->SetScreenResolution(Sizes[Index]);
		break;
	}
	case Quality:
		Display->SetOverallScalabilityLevel(FMath::Clamp(FMath::Max(Display->GetOverallScalabilityLevel(), 0) + Direction, 0, 3));   // low to epic
		break;
	case VSync:
		Display->SetVSyncEnabled(!Display->IsVSyncEnabled());
		break;
	case FrameLimit:
	{
		int32 Index = 0;
		for (int32 i = 0; i < UE_ARRAY_COUNT(FrameLimits); ++i)
		{
			Index = FrameLimits[i] <= Display->GetFrameRateLimit() ? i : Index;
		}
		Display->SetFrameRateLimit(FrameLimits[FMath::Clamp(Index + Direction, 0, static_cast<int32>(UE_ARRAY_COUNT(FrameLimits)) - 1)]);
		break;
	}
	case ViewAngle:
	case EyeHeight:
	case EyeForward:
		if (SaveData)
		{
			if (SettingsRow == ViewAngle)
			{
				// default, then 16 to 90 degrees in steps of 2: on a rig, 2 x atan(screen height / (2 x eye distance))
				const int32 Next = SaveData->OnboardFov == 0 ? (Direction > 0 ? 60 : 0) : SaveData->OnboardFov + Direction * 2;
				SaveData->OnboardFov = Next < 16 ? 0 : FMath::Min(Next, 90);
			}
			else if (SettingsRow == EyeHeight)
			{
				SaveData->EyeHeight = FMath::Clamp(SaveData->EyeHeight + Direction * 10, -200, 200);
			}
			else
			{
				SaveData->EyeForward = FMath::Clamp(SaveData->EyeForward + Direction * 10, -300, 300);
			}
			UGameplayStatics::SaveGameToSlot(SaveData, SaveSlot, 0);
		}
		return;
	case Volume:
		if (SaveData)
		{
			SaveData->MasterVolume = FMath::Clamp(SaveData->MasterVolume + Direction * 10, 0, 100);
			FApp::SetVolumeMultiplier(SaveData->MasterVolume / 100.0f);
			UGameplayStatics::SaveGameToSlot(SaveData, SaveSlot, 0);
		}
		return;
	default:
		Keys.Reset();
		ApplyKeys();
		return;
	}
	FitResolution(Display);
	Display->ApplySettings(/*bCheckForCommandLineOverrides=*/false);   // applies and saves (GameUserSettings.ini)
}

void AFSLapVehiclePawn::ApplySavedSettings()
{
	if (SaveData && !IsTestRun())
	{
		FApp::SetVolumeMultiplier(SaveData->MasterVolume / 100.0f);
		Keys = SaveData->Keys;
	}
	// the window as saved, never exclusive full screen; on a first run borderless full screen. Not for a run that asks
	// for a window on the command line (the tests).
	if (!FParse::Param(FCommandLine::Get(), TEXT("windowed")))
	{
		UGameUserSettings* Display = GEngine->GetGameUserSettings();
		const bool bFirstRun = !FPaths::FileExists(GGameUserSettingsIni);
		if (bFirstRun)
		{
			Display->SetFullscreenMode(EWindowMode::WindowedFullscreen);
		}
		if (FitResolution(Display) || bFirstRun)
		{
			Display->ApplySettings(false);
		}
	}
	ApplyKeys();
}

// The project's mappings, then each changed action's default key replaced by its new one.
void AFSLapVehiclePawn::ApplyKeys()
{
	const APlayerController* Player = Cast<APlayerController>(GetController());
	UPlayerInput* Input = Player ? Player->PlayerInput : nullptr;
	if (!Input)
	{
		return;
	}
	Input->ForceRebuildingKeyMaps(/*bRestoreDefaults=*/true);
	for (int32 Item = 0; Item < Keys.Num() && Item < EFSLapKey::Count; ++Item)
	{
		const FKeyItem& Action = KeyItem(Item);
		const FKey Key(Keys[Item]);
		if (Keys[Item].IsNone() || !Key.IsValid() || Key == Action.Default)
		{
			continue;
		}
		if (Action.Scale != 0)
		{
			Input->RemoveAxisMapping(FInputAxisKeyMapping(Action.Mapping, Action.Default, Action.Scale));
			Input->AddAxisMapping(FInputAxisKeyMapping(Action.Mapping, Key, Action.Scale));
		}
		else
		{
			Input->RemoveActionMapping(FInputActionKeyMapping(Action.Mapping, Action.Default));
			Input->AddActionMapping(FInputActionKeyMapping(Action.Mapping, Key));
		}
	}
	Input->ForceRebuildingKeyMaps(/*bRestoreDefaults=*/false);
	if (SaveData && !IsTestRun())
	{
		SaveData->Keys = Keys;
		UGameplayStatics::SaveGameToSlot(SaveData, SaveSlot, 0);
	}
}

void AFSLapVehiclePawn::UpdateKeyCapture()
{
	const APlayerController* Player = Cast<APlayerController>(GetController());
	if (CaptureItem < 0 || !Player)
	{
		return;
	}
	if (bCaptureFresh)   // the key press that started the capture is not the new key
	{
		bCaptureFresh = false;
		return;
	}
	TArray<FKey> All;
	EKeys::GetAllKeys(All);
	for (const FKey& Key : All)
	{
		if (Key.IsGamepadKey() || Key.IsMouseButton() || !Key.IsDigital() || Key == EKeys::AnyKey || !Player->WasInputKeyJustPressed(Key))
		{
			continue;
		}
		const int32 Item = CaptureItem;
		CaptureItem = -1;
		if (Key == EKeys::Escape)
		{
			return;
		}
		// the menus need their keys (the arrows also drive), and the numpad keys hold the side and front views
		if (Key == EKeys::Enter || Key == EKeys::Up || Key == EKeys::Down || Key == EKeys::Left || Key == EKeys::Right
			|| Key == EKeys::NumPadFour || Key == EKeys::NumPadSix || Key == EKeys::NumPadEight)
		{
			KeyMessage = Key.GetDisplayName(false).ToString().ToUpper() + TEXT(" is a menu or view key");
			return;
		}
		for (int32 Other = 0; Other < EFSLapKey::Count; ++Other)
		{
			if (Other != Item && GetKeyName(Other) == Key.GetDisplayName(false).ToString().ToUpper())
			{
				KeyMessage = GetKeyName(Other) + TEXT(" is already ") + KeyItem(Other).Label;
				return;
			}
		}
		Keys.SetNum(EFSLapKey::Count);
		Keys[Item] = Key.GetFName();
		ApplyKeys();
		return;
	}
}
