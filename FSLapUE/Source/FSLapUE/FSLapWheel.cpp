#include "FSLapWheel.h"

#include "Async/Async.h"
#include "Engine/Engine.h"
#include "Engine/GameViewportClient.h"
#include "HAL/PlatformTime.h"
#include "Widgets/SWindow.h"

#if PLATFORM_WINDOWS
#include "Windows/AllowWindowsPlatformTypes.h"
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include "Windows/HideWindowsPlatformTypes.h"
#endif

FFSLapWheel::~FFSLapWheel()
{
	if (Search.IsValid())
	{
		FOpened Late = Search.Get();   // waits for a search still running
		Release(Late);
	}
	Close();
}

void FFSLapWheel::Release(FOpened& Opened)
{
#if PLATFORM_WINDOWS
	if (Opened.Effect)
	{
		static_cast<IDirectInputEffect*>(Opened.Effect)->Stop();
		static_cast<IDirectInputEffect*>(Opened.Effect)->Release();
	}
	if (Opened.Device)
	{
		static_cast<IDirectInputDevice8W*>(Opened.Device)->Unacquire();
		static_cast<IDirectInputDevice8W*>(Opened.Device)->Release();
	}
	if (Opened.Input)
	{
		static_cast<IDirectInput8W*>(Opened.Input)->Release();
	}
#endif
	Opened = FOpened();
}

void FFSLapWheel::Close()
{
	Release(Current);
	Name.Empty();
}

FFSLapWheel::FOpened FFSLapWheel::Open(const FString& Preferred, void* Window, int32 ForceAxis)
{
	FOpened Result;
	Result.ForceAxis = ForceAxis;
#if PLATFORM_WINDOWS
	IDirectInput8W* DI = nullptr;
	if (FAILED(DirectInput8Create(GetModuleHandleW(nullptr), DIRECTINPUT_VERSION, IID_IDirectInput8W, reinterpret_cast<void**>(&DI), nullptr)))
	{
		return Result;
	}
	Result.Input = DI;
	struct FFound { GUID Guid; FString Name; bool bWheel; };
	TArray<FFound> Found;
	DI->EnumDevices(DI8DEVCLASS_GAMECTRL, [](LPCDIDEVICEINSTANCEW Info, LPVOID Out) -> BOOL
	{
		static_cast<TArray<FFound>*>(Out)->Add({ Info->guidInstance, Info->tszProductName, GET_DIDEVICE_TYPE(Info->dwDevType) == DI8DEVTYPE_DRIVING });
		return DIENUM_CONTINUE;
	}, &Found, DIEDFL_ATTACHEDONLY);
	const FFound* Pick = Found.FindByPredicate([&Preferred](const FFound& F) { return F.Name == Preferred; });
	Pick = Pick ? Pick : Found.FindByPredicate([](const FFound& F) { return F.bWheel; });
	Pick = Pick ? Pick : (Found.Num() ? &Found[0] : nullptr);
	IDirectInputDevice8W* D = nullptr;
	if (!Pick || FAILED(DI->CreateDevice(Pick->Guid, &D, nullptr)))
	{
		Release(Result);
		return Result;
	}
	Result.Device = D;
	Result.Name = Pick->Name;
	D->SetDataFormat(&c_dfDIJoystick2);
	DIDEVCAPS Caps = {};
	Caps.dwSize = sizeof(Caps);
	// force feedback needs exclusive access; a controller without it (pedals) stays shared as before
	bool bForce = ForceAxis >= 0 && ForceAxis < 8 && SUCCEEDED(D->GetCapabilities(&Caps)) && (Caps.dwFlags & DIDC_FORCEFEEDBACK);
	if (Window)
	{
		// background: the pedals keep working while another window (Moza Pit House) has the focus
		bForce = bForce && SUCCEEDED(D->SetCooperativeLevel(static_cast<HWND>(Window), DISCL_BACKGROUND | DISCL_EXCLUSIVE));
		if (!bForce)
		{
			D->SetCooperativeLevel(static_cast<HWND>(Window), DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);
		}
	}
	else
	{
		bForce = false;
	}
	if (bForce)
	{
		DIPROPDWORD Center = {};
		Center.diph.dwSize = sizeof(DIPROPDWORD);
		Center.diph.dwHeaderSize = sizeof(DIPROPHEADER);
		Center.diph.dwHow = DIPH_DEVICE;
		Center.dwData = DIPROPAUTOCENTER_OFF;   // the model's torque does the centring
		D->SetProperty(DIPROP_AUTOCENTER, &Center.diph);
	}
	DIPROPRANGE Range = {};
	Range.diph.dwSize = sizeof(DIPROPRANGE);
	Range.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	Range.diph.dwHow = DIPH_DEVICE;   // every axis
	Range.lMin = -1000;
	Range.lMax = 1000;
	D->SetProperty(DIPROP_RANGE, &Range.diph);
	D->Acquire();
	if (bForce)
	{
		static const DWORD Offsets[] = { DIJOFS_X, DIJOFS_Y, DIJOFS_Z, DIJOFS_RX, DIJOFS_RY, DIJOFS_RZ, DIJOFS_SLIDER(0), DIJOFS_SLIDER(1) };   // Read's axis order
		DWORD Axis = Offsets[ForceAxis];
		LONG Direction = 0;
		DICONSTANTFORCE Constant = { 0 };   // starts at no force
		DIEFFECT Effect = {};
		Effect.dwSize = sizeof(DIEFFECT);
		Effect.dwFlags = DIEFF_CARTESIAN | DIEFF_OBJECTOFFSETS;
		Effect.dwDuration = INFINITE;
		Effect.dwGain = DI_FFNOMINALMAX;
		Effect.dwTriggerButton = DIEB_NOTRIGGER;
		Effect.cAxes = 1;
		Effect.rgdwAxes = &Axis;
		Effect.rglDirection = &Direction;
		Effect.cbTypeSpecificParams = sizeof(DICONSTANTFORCE);
		Effect.lpvTypeSpecificParams = &Constant;
		IDirectInputEffect* E = nullptr;
		if (SUCCEEDED(D->CreateEffect(GUID_ConstantForce, &Effect, &E, nullptr)))
		{
			Result.Effect = E;
			E->Start(1, 0);
		}
	}
#endif
	return Result;
}

void FFSLapWheel::SetForce(float Force)
{
#if PLATFORM_WINDOWS
	if (!Current.Effect)
	{
		return;
	}
	DICONSTANTFORCE Constant = { FMath::RoundToInt(FMath::Clamp(Force, -1.0f, 1.0f) * DI_FFNOMINALMAX) };
	DIEFFECT Effect = {};
	Effect.dwSize = sizeof(DIEFFECT);
	Effect.dwFlags = DIEFF_CARTESIAN | DIEFF_OBJECTOFFSETS;
	Effect.cbTypeSpecificParams = sizeof(DICONSTANTFORCE);
	Effect.lpvTypeSpecificParams = &Constant;
	static_cast<IDirectInputEffect*>(Current.Effect)->SetParameters(&Effect, DIEP_TYPESPECIFICPARAMS | DIEP_START);
#endif
}

bool FFSLapWheel::Read(const FString& Preferred, TArray<float>& Axes, int32 ForceAxis)
{
#if PLATFORM_WINDOWS
	if (Current.Device && Current.ForceAxis != ForceAxis)
	{
		Close();   // force feedback switched on or off: open it again, exclusive or shared
		NextTry = 0;
	}
	if (!Current.Device)
	{
		if (Search.IsValid())
		{
			if (!Search.IsReady())
			{
				return false;
			}
			Current = Search.Get();
			Search = TFuture<FOpened>();
			Name = Current.Name;
		}
		else if (FPlatformTime::Seconds() >= NextTry)
		{
			NextTry = FPlatformTime::Seconds() + 2;
			const TSharedPtr<SWindow> GameWindow = GEngine && GEngine->GameViewport ? GEngine->GameViewport->GetWindow() : nullptr;
			void* Window = GameWindow && GameWindow->GetNativeWindow() ? GameWindow->GetNativeWindow()->GetOSWindowHandle() : nullptr;
			Search = Async(EAsyncExecution::Thread, [Preferred, Window, ForceAxis] { return Open(Preferred, Window, ForceAxis); });
		}
		if (!Current.Device)
		{
			return false;
		}
	}
	IDirectInputDevice8W* D = static_cast<IDirectInputDevice8W*>(Current.Device);
	DIJOYSTATE2 State;
	D->Poll();
	if (FAILED(D->GetDeviceState(sizeof(State), &State)))
	{
		if (FAILED(D->Acquire()))
		{
			Close();   // unplugged: look for it again
		}
		else if (Current.Effect)
		{
			static_cast<IDirectInputEffect*>(Current.Effect)->Start(1, 0);   // acquiring again drops the effect from the device
		}
		return false;
	}
	const LONG Raw[] = { State.lX, State.lY, State.lZ, State.lRx, State.lRy, State.lRz, State.rglSlider[0], State.rglSlider[1] };
	Axes.SetNum(UE_ARRAY_COUNT(Raw));
	for (int32 i = 0; i < Axes.Num(); ++i)
	{
		Axes[i] = Raw[i] / 1000.0f;
	}
	// temporary (owner, 2026-11-14: no cable for the pedals): the buttons follow as axes 0 or 1, so the wheel setup can
	// take a held button as throttle or brake like a pedal; remove this block with the pedals connected
	for (int32 i = 0; i < Buttons; ++i)
	{
		Axes.Add(State.rgbButtons[i] & 0x80 ? 1.0f : 0.0f);
	}
	return true;
#else
	return false;
#endif
}
