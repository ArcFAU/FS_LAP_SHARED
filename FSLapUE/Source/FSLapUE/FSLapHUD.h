#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "FSLapHUD.generated.h"

class AFSLapVehiclePawn;
class UFont;
namespace HoMoLap { class Simulation; }

/** The web game's HUD layout: timing tower, map, speedometer, g-g plot and aero readouts, start lights and big messages. */
UCLASS()
class FSLAPUE_API AFSLapHUD : public AHUD
{
	GENERATED_BODY()

public:
	virtual void DrawHUD() override;

private:
	// All positions and sizes are for a 1080-line screen and scaled from there.
	void Label(const FString& Text, float X, float Y, float Size, const FLinearColor& Color, bool bBold = false, bool bCentre = false);
	void Box(float X, float Y, float W, float H, const FLinearColor& Color);
	void Panel(float X, float Y, float W, float H);
	void Slant(float X, float Y, float W, float H, const FLinearColor& Color, float Lean = 10);   // a parallelogram: the top edge is shifted right by Lean
	void Ring(float X, float Y, float Radius, const FLinearColor& Color);
	void DrawMenu(const AFSLapVehiclePawn& Vehicle);
	void DrawSetup(const AFSLapVehiclePawn& Vehicle);
	void DrawWheel(const AFSLapVehiclePawn& Vehicle);
	void DrawTower(const AFSLapVehiclePawn& Vehicle, const HoMoLap::Simulation& Sim);
	void DrawMinimap(const HoMoLap::Simulation& Sim);
	void DrawSpeedo(const HoMoLap::Simulation& Sim);
	void DrawAero(const HoMoLap::Simulation& Sim);
	void DrawLights(const AFSLapVehiclePawn& Vehicle);
	void UpdateMessages(const AFSLapVehiclePawn& Vehicle, const HoMoLap::Simulation& Sim, float DeltaSeconds);
	void ShowMessage(const FString& Title, const FString& Sub, const FLinearColor& Color, float Seconds, bool bSmall);
	void Button(const FString& Text, float X, float Y, float W, float H, int32 Target);   // a clickable box, lit while the mouse is over it
	void UpdateMouse(AFSLapVehiclePawn& Vehicle, bool bMenus);   // the cursor in the menus; hover and clicks go to the pawn

	TArray<TPair<FBox2f, int32>> Targets;   // what this frame drew that the mouse can click, in layout units
	FVector2f LastMouse = FVector2f::ZeroVector;   // on the screen (a second monitor can be at negative positions)
	bool bMouseSeen = false;
	int32 HoverTarget = -2;   // under the mouse last frame; -2: nothing (MouseBack is -1)

	UPROPERTY()
	UFont* Font = nullptr;

	float S = 1;   // screen scale
	float ScreenW = 1920, ScreenH = 1080;
	float WrongWayTime = 0;

	FString MessageTitle, MessageSub;
	FLinearColor MessageColor = FLinearColor::White;
	float MessageTime = 0;
	bool bMessageSmall = false;

	// what the simulation looked like last frame, to turn changes into messages
	const HoMoLap::Simulation* LastSim = nullptr;
	double LastSplits[2] = { 0, 0 };
	size_t LastLaps = 0;
	int32 LastCones = 0, LastOff = 0;
	bool bLastValid = true, bLastGot100 = false, bLastGot75 = false;
	uint8 LastFlow = 0;

	TArray<FVector2f> GTrail;
};
