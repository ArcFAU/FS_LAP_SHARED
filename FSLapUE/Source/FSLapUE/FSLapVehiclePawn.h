#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "HoMoLapCore.h"
#include <memory>
#include "FSLapLog.h"
#include "FSLapLts.h"
#include "FSLapWheel.h"
#include "FSLapVehiclePawn.generated.h"

class UAudioComponent;
class USoundWave;
class UCameraComponent;
class UInstancedStaticMeshComponent;
class UStaticMeshComponent;

enum class EFSLapFlow : uint8 { Menu, Setup, Countdown, Race, Paused, Replay, Wheel };   // Wheel: the steering wheel setup, from the setup screen

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
	int32 GetMenuIndex() const { return MenuIndex; }
	int32 GetLights() const { return FMath::Min(5, FMath::FloorToInt32(CountdownTime / 0.6f)); }
	double GetBestLap() const { return CurrentBest().Time; }   // 0 until a valid lap is set
	double GetBestSplit(int32 Index) const { return Index == 0 ? CurrentBest().Split1 : CurrentBest().Split2; }   // 0 if the best lap has none
	bool GetGapToBest(double& Gap) const;   // live time against the best lap; false when there is none
	void StartRace(HoMoLap::Mode NewMode);
	void CycleCamera();   // chase or onboard
	float GetMotorTone() const { return MotorTone; }   // the upper gear tone in Hz, which the motor sound follows
	bool CanReplay() const { return CurrentBest().Ghost.size() > 30; }
	void StartReplay();   // the best lap of this mode and setup, seen from roadside cameras
	void StopReplay();
	bool IsOnboard() const { return bOnboard; }
	int32 GetSetupRowCount() const;
	int32 GetSetupRow() const { return SetupRow; }
	FString GetSetupRowText(int32 Row) const;
	FString GetWheelText() const;   // the wheel setup screen: the step, what the wheel reads now
	FString GetLtsLabel() const;    // the HUD's name for the reference lap

	// Used by Dev\FSLapSliceTest: the AI drives, and the car can be held still for the photos.
	bool bAutopilot = false;
	bool bFrozen = false;
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
		char State;   // 0 standing, 1 tipped, 2 hidden (free-roam cones outside free roam)
	};
	TArray<FConeInstance> Cones;   // same order as Track.cones

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

	const HoMoLap::Track& ActiveTrack() const { return RaceMode == HoMoLap::Mode::City ? CityTrack : Track; }

	HoMoLap::Track Track;
	HoMoLap::Track CityTrack;
	TArray<USceneComponent*> CityParts;   // what the city shows: ground, buildings, lamps, trees
	HoMoLap::Setup Setup;
	TUniquePtr<HoMoLap::Simulation> Simulation;
	USceneComponent* GhostBest = nullptr;
	USceneComponent* GhostLts = nullptr;
	UStaticMeshComponent* Ground = nullptr;
	UStaticMeshComponent* Paddock = nullptr;
	TArray<UInstancedStaticMeshComponent*> PaddockSets;   // the FSG paddock: shown in hot lap and arcade
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
	bool UsesAdtm(HoMoLap::Mode Mode) const { return bAdtm && Mode != HoMoLap::Mode::Arcade && Mode != HoMoLap::Mode::City; }
	FString KernelName(HoMoLap::Mode Mode) const;
	void RequestLts();
	void ApplyLts(const FFSLapLts::FLap& Lap);
	TArray<TPair<FString, double>> LtsOverrides() const;
	FString SetupJson() const;
	FString ReferenceJson() const;
	TArray<float> WheelAxes, WheelRest, WheelNew;   // WheelNew: the calibration being recorded
	bool bWheelLive = false;
	float WheelSteer = 0, WheelThrottle = 0, WheelBrake = 0;
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
	bool bOnboard = false;
	bool bCameraSnap = true;
	double CameraYaw = 0;
	FVector CameraPosition = FVector::ZeroVector;
};
