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

FFSLapWheel::FOpened FFSLapWheel::Open(const FString& Preferred, void* Window)
{
	FOpened Result;
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
	if (Window)
	{
		// background: the pedals keep working while another window (Moza Pit House) has the focus
		D->SetCooperativeLevel(static_cast<HWND>(Window), DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);
	}
	DIPROPRANGE Range = {};
	Range.diph.dwSize = sizeof(DIPROPRANGE);
	Range.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	Range.diph.dwHow = DIPH_DEVICE;   // every axis
	Range.lMin = -1000;
	Range.lMax = 1000;
	D->SetProperty(DIPROP_RANGE, &Range.diph);
	D->Acquire();
#endif
	return Result;
}

bool FFSLapWheel::Read(const FString& Preferred, TArray<float>& Axes)
{
#if PLATFORM_WINDOWS
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
			Search = Async(EAsyncExecution::Thread, [Preferred, Window] { return Open(Preferred, Window); });
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
		return false;
	}
	const LONG Raw[] = { State.lX, State.lY, State.lZ, State.lRx, State.lRy, State.lRz, State.rglSlider[0], State.rglSlider[1] };
	Axes.SetNum(UE_ARRAY_COUNT(Raw));
	for (int32 i = 0; i < Axes.Num(); ++i)
	{
		Axes[i] = Raw[i] / 1000.0f;
	}
	return true;
#else
	return false;
#endif
}
