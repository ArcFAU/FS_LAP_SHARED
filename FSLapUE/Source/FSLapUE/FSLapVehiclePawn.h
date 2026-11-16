#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "HoMoLapCore.h"
#include <memory>
#include "FSLapLog.h"
#include "FSLapLts.h"
#include "FSLapWheel.h"
#include "FSLapSil.h"
#include "FSLapVehiclePawn.generated.h"

class UAudioComponent;
class USoundWave;
class UCameraComponent;
class UInstancedStaticMeshComponent;
class UStaticMeshComponent;

enum class EFSLapFlow : uint8 { Menu, Setup, Countdown, Race, Paused, Replay, Wheel, Settings };   // Wheel: the steering wheel setup, from the setup screen; Settings: display, sound and keys, from the menu

// The driving actions whose key the settings screen can change (the order of its key rows and of the saved keys).
namespace EFSLapKey { enum Type { Throttle, Brake, SteerLeft, SteerRight, Handbrake, Reset, Camera, Pause, Replay, Menu, Count }; }

UCLASS()
class FSLAPUE_API AFSLapVehiclePawn : public APawn
{
	GENERATED_BODY()

public:
	AFSLapVehiclePawn();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	const HoMoLap::Simulation* GetSimulation() const { return Simulation.Get(); }
	EFSLapFlow GetFlow() const { return Flow; }
	HoMoLap::Mode GetRaceMode() const { return RaceMode; }
	int32 GetMap() const { return Map; }   // the event track driven now: 0 FSG, then HoMoLap::MakeEventTrack's order
	int32 GetChosenMap() const { return ChosenMap; }   // the one chosen in the menu's TRACK row
	int32 GetMapCount() const { return 1 + static_cast<int32>(Others.size()); }
	int32 FindMap(const TCHAR* Id) const;   // by HoMoLap::Track::id ("fsg", "fscz", "fsf"); 0 if unknown
	FString GetMapName(int32 i) const { return UTF8_TO_TCHAR(MapTrack(i).name); }
	FString GetMapNote(int32 i) const;   // where the track comes from and whose lap the reference is
	static int32 GetMenuTrackRow();
	int32 GetMenuIndex() const { return MenuIndex; }
	// The main menu: the ways to drive, then setup, settings and quit (one table for the pawn and the HUD).
	static int32 GetMenuRowCount();
	static const TCHAR* GetMenuName(int32 Row);
	static const TCHAR* GetMenuNote(int32 Row);
	int32 GetLights() const { return FMath::Min(5, FMath::FloorToInt32(CountdownTime / 0.6f)); }
	double GetBestLap() const { return CurrentBest().Time; }   // 0 until a valid lap is set
	double GetBestSplit(int32 Index) const { return Index == 0 ? CurrentBest().Split1 : CurrentBest().Split2; }   // 0 if the best lap has none
	bool GetGapToBest(double& Gap) const;   // live time against the best lap; false when there is none
	void StartRace(HoMoLap::Mode NewMode, int32 NewMap = 0, bool bNewSil = false);
	bool IsSil() const { return bSil; }   // the SIL mode: the hot lap with a VCU driving the motors (FSLapSil)
	FString GetSilStatus() const { return Sil.Status(); }
	void CycleCamera();   // chase, cockpit, hood, nose
	float GetMotorTone() const { return MotorTone; }   // the upper gear tone in Hz, which the motor sound follows
	bool CanReplay() const { return CurrentBest().Ghost.size() > 30; }
	void StartReplay();   // the best lap of this mode and setup, seen from roadside cameras
	void StopReplay();
	bool IsOnboard() const { return CameraView == 1; }   // the cockpit view
	int32 GetCameraView() const { return CameraView; }   // 0 chase, 1 cockpit, 2 hood, 3 nose
	int32 GetHeldView() const { return HeldView; }   // numpad key held: 4 left, 6 right, 8 front; 0 none
	// The setup screen's rows; in the Settings flow, the settings screen's (FSLapSettings.cpp): the HUD draws both the same way.
	int32 GetSetupRowCount() const;
	int32 GetSetupRow() const { return Flow == EFSLapFlow::Settings ? SettingsRow : SetupRow; }
	FString GetSetupRowText(int32 Row) const;
	FString GetKeyName(int32 Item) const;   // the key a driving action has now (EFSLapKey order), for the HUD's key help
	FString GetWheelText() const;   // the wheel setup screen: the step, what the wheel reads now
	FString GetLtsLabel() const;    // the HUD's name for the reference lap

	// The mouse in the menus (the HUD finds what is under it): a row of the menu or setup, the pause items in their key order, or back.
	static constexpr int32 MouseBack = -1;
	void MouseHover(int32 Target);
	void MouseClick(int32 Target, bool bRightButton);

	// Used by Dev\FSLapSliceTest: the AI drives, and the car can be held still for the photos.
	bool bAutopilot = false;
	bool bFrozen = false;
	int32 TestWheelButton = -1;   // >= 0: that button of a pretend wheel is held (index into the wheel's axes, as WheelButtons)
	void KnockDownConeForTest(int32 Index) { Simulation->coneDown[Index] = 1; }
	int32 GetTrafficCount() const { return RaceMode == HoMoLap::Mode::City ? TrafficCars.Num() : 0; }
	bool GetTrafficCar(FVector& Location) const;   // where the first traffic car is; false outside the cities

private:
	friend class UFSLapSliceTest;   // the acceptance test steers the menu directly
	UPROPERTY(VisibleAnywhere)
	USceneComponent* VehicleRoot;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Body;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* WheelFL;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* WheelFR;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* WheelRL;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* WheelRR;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* SteeringWheel;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* CadHubFL = nullptr;   // the front uprights, calipers and motors: they steer with the wheel

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* CadHubFR = nullptr;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* ChaseCamera;

	void Throttle(float Value);
	void Brake(float Value);
	void Steer(float Value);
	void HandbrakePressed();
	void HandbrakeReleased();
	void ResetPressed();
	void MenuUp();
	void MenuDown();
	void MenuLeft();
	void MenuRight();
	void ChangeSetup(int32 Direction);
	// FSLapSettings.cpp: the settings screen (display, sound, keys)
	int32 GetSettingsRowCount() const;
	FString GetSettingsRowText(int32 Row) const;
	void ChangeSettings(int32 Direction);
	void ApplySavedSettings();    // the saved volume and keys, once the save and the player's input exist
	void ApplyKeys();
	void UpdateKeyCapture();      // while a key row waits for its new key
	// the wheel's buttons: one per driving action, set on the settings screen like the keys (a key row that waits takes a
	// wheel button too). On the press for the actions, held for the pedals and steering.
	void UpdateWheelButtons(const TArray<float>& Axes);
	int32 WheelButtonFor(int32 Item) const { return WheelButtons.IsValidIndex(Item) ? WheelButtons[Item] : -1; }
	bool IsCapturingKey() const { return CaptureItem >= 0; }
	void ClearEffects();
	void ReplayPressed();
	void MakeSounds();
	void MakeTraffic();
	void SpawnTrafficInstances();   // the active city's cars into the shared instance sets
	void UpdateTraffic(float DeltaSeconds);
	void UpdateSound(float DeltaSeconds);
	void UpdateReplay(float DeltaSeconds);
	void UpdateEffects(float DeltaSeconds);
	void Confirm();
	void TogglePause();
	void BackToMenu();
	void RestartSimulation(HoMoLap::Mode NewMode);
	void EnterMenu();
	void Simulate();
	void UpdateVisuals();
	void UpdateCones();
	void UpdateGhosts();
	void CaptureBest();
	void UpdateCamera(float DeltaSeconds);
	void SpawnScenery();
	USceneComponent* MakeGhost(AActor* Holder, const TCHAR* MaterialPath);

	struct FConeInstance
	{
		UInstancedStaticMeshComponent* Set;
		int32 Instance;
		FTransform Upright;
		char State;   // 0 standing, 1 tipped, 2 hidden (free-roam cones outside free roam, the other map's cones)
		int32 Map;    // whose cone (MapTrack)
		int32 Index;  // in that track's cones
	};
	TArray<FConeInstance> Cones;   // both FSG tracks' cones

	struct FBest
	{
		double Time = 0, Penalty = 0, Split1 = 0, Split2 = 0;
		std::vector<HoMoLap::GhostPoint> Ghost;
		std::vector<double> Progress;   // distance along the lap of each ghost sample
	};
	TMap<FString, FBest> Bests;   // one per mode and setup, loaded from and saved to disk
	FString CurrentKey;
	FString MakeBestKey() const;
	void LoadBests();
	void BuildProgress(FBest& Best, const HoMoLap::Track& OnTrack) const;
	void SaveBest(const FString& Key, const FBest& Best);
	const FBest& CurrentBest() const;

	UPROPERTY()
	class UFSLapSave* SaveData = nullptr;

	UPROPERTY()
	TArray<UAudioComponent*> MotorSounds;

	UPROPERTY()
	UAudioComponent* TyreSound = nullptr;

	UPROPERTY()
	TArray<UInstancedStaticMeshComponent*> TrafficSets;   // car bodies by colour, then the cabins

	UPROPERTY()
	UAudioComponent* WindSound = nullptr;

	UPROPERTY()
	UAudioComponent* GrassSound = nullptr;

	UPROPERTY()
	USoundWave* ConeHitSound = nullptr;

	UPROPERTY()
	USoundWave* BeepSound = nullptr;

	const HoMoLap::Track& MapTrack(int32 i) const { return i <= 0 || i > static_cast<int32>(Others.size()) ? Track : Others[i - 1]; }
	const HoMoLap::Track& ActiveTrack() const { return RaceMode == HoMoLap::Mode::City ? CityTrack : MapTrack(Map); }
	void ChooseMap(int32 Direction);   // the menu's TRACK row
	// the ground under a point (m) and the car's attitude on it there (pitch nose up +, roll as Unreal's): 0 and level on a
	// flat track and in the city
	double GroundAt(double x, double y) const { return RaceMode == HoMoLap::Mode::City ? 0.0 : ActiveTrack().HeightAt(x, y); }
	FRotator GroundAttitude(double x, double y, double h) const;

	HoMoLap::Track Track;
	std::vector<HoMoLap::Track> Others;   // the other events' tracks (HoMoLap::MakeEventTrack 1, 2, ...)
	int32 Map = 0, ChosenMap = 0;   // the track driven now; the one chosen in the menu (hot lap, SIL and arcade)
	HoMoLap::Track CityTrack;
	TArray<USceneComponent*> CityParts;   // what the city shows: ground, buildings, lamps, trees
	HoMoLap::Setup Setup;
	TUniquePtr<HoMoLap::Simulation> Simulation;
	USceneComponent* GhostBest = nullptr;
	USceneComponent* GhostLts = nullptr;
	UStaticMeshComponent* Ground = nullptr;
	TArray<UStaticMeshComponent*> OtherGrounds;   // the other events' grounds, in Others' order
	UStaticMeshComponent* Paddock = nullptr;
	TArray<UInstancedStaticMeshComponent*> PaddockSets;   // the FSG paddock: shown in hot lap and arcade
	TArray<TArray<UInstancedStaticMeshComponent*>> OtherPaddocks;   // the same paddock around each of the others
	UInstancedStaticMeshComponent* Skids = nullptr;
	UInstancedStaticMeshComponent* Smoke = nullptr;
	struct FPuff
	{
		FVector2D Position = FVector2D::ZeroVector;
		float Life = 0;
		float Radius = 0;
	};
	TArray<FPuff> Puffs;
	FVector2D LastWheel[4];
	bool bWheelSkidding[4] = { false, false, false, false };
	int32 SkidHead = 0;
	int32 PuffHead = 0;
	bool bPuffsActive = false;
	int32 SetupRow = 0;
	int32 SettingsRow = 0;
	TArray<FName> Keys;        // the key of each driving action (EFSLapKey order); none: the project's default
	TArray<int32> WheelButtons;   // the wheel button of each driving action (EFSLapKey order); -1 or missing: none
	TArray<float> WheelButtonsBefore;   // last frame's axes, so a button acts on the press
	int32 CaptureItem = -1;    // the action whose key row waits for a key press
	bool bCaptureFresh = false;   // the ENTER that started the capture is still down this frame
	FString KeyMessage;        // why the last key was refused
	bool bDriverAssist = true;
	int32 PresetIndex = 0;   // -1 once the aero rows have been changed by hand
	EFSLapFlow Flow = EFSLapFlow::Menu;
	EFSLapFlow ResumeTo = EFSLapFlow::Race;
	HoMoLap::Mode RaceMode = HoMoLap::Mode::Sim;
	int32 MenuIndex = 0;
	float CountdownTime = 0;
	float CountdownHold = 0;
	double ReplayTime = 0, ReplaySpeed = 0;
	float MotorTone = 0, SoundLevel = 0, SoundSqueal = 0, SoundGrass = 0;
	int32 HeardLights = 0;

	struct FTrafficPath { TArray<FVector2D> P; TArray<double> S; };   // a closed street loop in metres; S holds the distance at each point
	struct FTrafficCar { int32 Path; double Along; int32 Set; int32 Body = -1; float Yaw = 0; };
	TArray<FTrafficPath> TrafficPaths;
	TArray<FTrafficCar> TrafficCars;
	size_t TrafficWalls = 0;   // where the cars start in the city's walls
	FFSLapWheel Wheel;
	// the reference lap: the LTS lap stored with the track, or one the real LTS computes for this setup (FSLapLts)
	FFSLapLts Lts;
	int32 LtsModel = 0;           // index into FFSLapLts::Models; 0 the stored lap
	FString LtsAskedFor;          // the model and setup of the last request, so the same job is not queued twice
	FString LtsShown;             // the model of the reference shown now; empty for the stored lap
	std::vector<double> StoredLtsT, StoredLtsS;
	double StoredLtsLap = 0, StoredLtsSplit[2] = { 0, 0 }, StoredLtsScale = 1;
	FFSLapLog Log;                // every lap at 600 Hz to Saved/Runs
	// the vehicle model: FS Lap's own, or the team's ADTM compiled in (HoMoLapAdtm.h) for hot lap and free roam; arcade and city stay FS Lap's
	bool bAdtm = false;
	std::unique_ptr<HoMoLap::Simulation::VehicleModel> Adtm;   // the Simulation points at it
	bool UsesAdtm(HoMoLap::Mode Mode) const { return (bAdtm || bSil) && Mode != HoMoLap::Mode::Arcade && Mode != HoMoLap::Mode::City; }   // the SIL needs the ADTM
	bool bSil = false;
	FFSLapSil Sil;
	double SilTorque[4] = {};   // the inverters' torques this step; the Simulation points at it in the SIL mode
	FString KernelName(HoMoLap::Mode Mode) const;
	void RequestLts();
	void ApplyLts(const FFSLapLts::FLap& Lap);
	TArray<TPair<FString, double>> LtsOverrides() const;
	FString SetupJson() const;
	FString ReferenceJson() const;
	TArray<float> WheelAxes, WheelRest, WheelNew;   // WheelNew: the calibration being recorded
	bool bWheelLive = false;
	float WheelSteer = 0, WheelThrottle = 0, WheelBrake = 0;
	float WheelForce = 0;   // force feedback sent last frame, -1..1 of the wheel's strongest
	float ForceTestTime = 0;   // the wheel setup's test force: seconds left of a gentle push each way
	int32 WheelStep = 0;
	FString WheelMessage;
	bool IsWheelSetUp() const;
	void WheelNext();
	EFSLapFlow ReplayReturn = EFSLapFlow::Paused;
	size_t SeenLaps = 0;
	float ThrottleInput = 0;
	float BrakeInput = 0;
	float SteerInput = 0;
	bool Handbrake = false;
	double FixedAccumulator = 0;
	float WheelRotation = 0;
	float BodyPitch = 0, BodyRoll = 0, BodyPitchRate = 0, BodyRollRate = 0;
	int32 CameraView = 0;   // 0 chase, 1 cockpit (the driver's eyes), 2 hood, 3 nose
	int32 HeldView = 0;   // while numpad 4, 6 or 8 is held: the car from its left, right or front
	bool bCameraSnap = true;
	double CameraYaw = 0;
	FVector CameraPosition = FVector::ZeroVector;
};
