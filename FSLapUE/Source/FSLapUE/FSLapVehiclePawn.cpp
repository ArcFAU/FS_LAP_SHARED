#include "FSLapVehiclePawn.h"

#include "Camera/CameraComponent.h"
#include "Components/AudioComponent.h"
#include "Components/InputComponent.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/DirectionalLight.h"
#include "Engine/StaticMesh.h"
#include "Engine/StaticMeshActor.h"
#include "EngineUtils.h"
#include "FSLapSave.h"
#include "Kismet/GameplayStatics.h"
#include "Materials/MaterialInterface.h"
#include "Sound/SoundWave.h"
#include "UObject/ConstructorHelpers.h"

#include "Algo/BinarySearch.h"
#include "Algo/Reverse.h"

#include <algorithm>
#include <cmath>

namespace
{
	constexpr float MetersToCentimeters = 100.0f;
	constexpr float HubHeight = 20.0f;   // the body mesh has the ground at z = 0, the wheel meshes their hub
	const HoMoLap::Mode MenuModes[] = { HoMoLap::Mode::Sim, HoMoLap::Mode::Arcade, HoMoLap::Mode::City, HoMoLap::Mode::Free };
	const FVector SteeringWheelPosition(32.7, 0, 52);   // cm from the mid-axle ground point: the team's wheel where the CAD has it (data\car\steering_wheel.json)
	const TCHAR* const SaveSlot = TEXT("FSLap");
	const FVector DriverEye(-12, 0, 85);              // the headrest cushion ends at x = -35 cm and is centred at z = 73 cm
	constexpr float SteeringWheelTilt = -11.8f, SteeringWheelTravel = 140;   // tilt: the CAD's column angle
	constexpr int32 MaxSkids = 1500, MaxPuffs = 48;
	constexpr int32 MenuSetupRow = UE_ARRAY_COUNT(MenuModes);   // the row after the modes

	// the motor loops from the test-day recordings (tools\make_sound_loops.py), each with the value of the upper gear tone it was cut at
	struct FMotorLoop { const TCHAR* Asset; float ToneHz; };
	const FMotorLoop MotorLoops[] = { { TEXT("A_MotorLow"), 800 }, { TEXT("A_MotorMid"), 1930 }, { TEXT("A_MotorHigh"), 3860 } };
	// shortcut: the gear tooth counts are not in any of our data, so the upper tone is assumed to be 15 times the motor's revolutions per second; replace it once the real count is known.
	constexpr float MotorToneOrder = 15;

	// the setup editor: hotlap.setups fields, in the units of the web game's editor
	struct FSetupField { double HoMoLap::Setup::* Value; const TCHAR* Label; const TCHAR* Unit; double Min, Max, Step, Factor; int32 Decimals; };
	const FSetupField SetupFields[] = {
		{ &HoMoLap::Setup::ClA, TEXT("Downforce ClA"), TEXT("m2"), 0, 10, 0.05, 1, 2 },
		{ &HoMoLap::Setup::CdA, TEXT("Drag CdA"), TEXT("m2"), 0.6, 3, 0.05, 1, 2 },
		{ &HoMoLap::Setup::bal, TEXT("Aero balance, front"), TEXT("%"), 0.3, 0.7, 0.01, 100, 0 },
		{ &HoMoLap::Setup::mCar, TEXT("Car mass"), TEXT("kg"), 140, 280, 1, 1, 0 },
		{ &HoMoLap::Setup::mDrv, TEXT("Driver mass"), TEXT("kg"), 50, 110, 1, 1, 0 },
		{ &HoMoLap::Setup::wf, TEXT("Weight on the front axle"), TEXT("%"), 0.4, 0.6, 0.005, 100, 1 },
		{ &HoMoLap::Setup::h, TEXT("CoG height"), TEXT("mm"), 0.2, 0.4, 0.001, 1000, 0 },
		{ &HoMoLap::Setup::Pmax, TEXT("System power limit"), TEXT("kW"), 20, 120, 1, 1, 0 },
		{ &HoMoLap::Setup::Tm, TEXT("Motor torque, each of 4"), TEXT("N m"), 8, 30, 0.5, 1, 1 },
		{ &HoMoLap::Setup::G, TEXT("Gear ratio"), TEXT(": 1"), 8, 18, 0.05, 1, 2 },
		{ &HoMoLap::Setup::mu0, TEXT("Peak mu at 600 N"), TEXT(""), 1, 2, 0.01, 1, 2 },
		{ &HoMoLap::Setup::muExp, TEXT("Load sensitivity exponent"), TEXT(""), -0.3, 0, 0.01, 1, 2 },
		{ &HoMoLap::Setup::ap, TEXT("Peak slip angle"), TEXT("deg"), 4, 12, 0.1, 1, 1 },
		{ &HoMoLap::Setup::brakeFront, TEXT("Brake balance, front"), TEXT("%"), 0.5, 0.85, 0.01, 100, 0 },
		{ &HoMoLap::Setup::Fb, TEXT("Maximum brake force"), TEXT("N"), 4000, 14000, 100, 1, 0 },
		{ &HoMoLap::Setup::rollFront, TEXT("Roll stiffness, front"), TEXT("%"), 0.3, 0.8, 0.01, 100, 0 },
		{ &HoMoLap::Setup::tv, TEXT("Torque-vectoring authority"), TEXT("%"), 0, 2, 0.05, 100, 0 },
		{ &HoMoLap::Setup::delMax, TEXT("Steering lock"), TEXT("deg"), 15, 35, 0.5, 1, 1 },
	};
	struct FAeroPreset { const TCHAR* Name; double ClA, CdA, Bal; };
	const FAeroPreset AeroPresets[] = {
		{ TEXT("Tau 2026, LTS vehicle file"), 5.40, 1.70, 0.50 },
		{ TEXT("Tau 2026, vehicle-params"), 4.90, 1.40, 0.55 },
		{ TEXT("Upsilon 2027, CFD baseline 1011"), 5.64, 1.90, 0.50 },
		{ TEXT("Upsilon 2027, CFD best V4_K"), 6.94, 1.96, 0.45 },
		{ TEXT("Tau 2026, no downforce"), 0.00, 1.70, 0.50 },
	};

	// three.js fields of view are vertical, Unreal's are horizontal; shortcut: assumes a 16:9 window.
	float HorizontalFov(float VerticalDegrees)
	{
		return FMath::RadiansToDegrees(2 * FMath::Atan(FMath::Tan(FMath::DegreesToRadians(VerticalDegrees) / 2) * 16.0f / 9.0f));
	}
}

AFSLapVehiclePawn::AFSLapVehiclePawn()
{
	PrimaryActorTick.bCanEverTick = true;
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	VehicleRoot = CreateDefaultSubobject<USceneComponent>(TEXT("VehicleRoot"));
	RootComponent = VehicleRoot;

	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	Body->SetupAttachment(VehicleRoot);
	Body->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> BodyAsset(
		TEXT("/Game/FS/Generated/Car/SM_Body/StaticMeshes/SM_Body.SM_Body"));
	if (BodyAsset.Succeeded())
	{
		Body->SetStaticMesh(BodyAsset.Object);
	}

	auto setupWheel = [this](UStaticMeshComponent*& Wheel, const TCHAR* Name, const TCHAR* AssetPath, const FVector& Position)
	{
		Wheel = CreateDefaultSubobject<UStaticMeshComponent>(Name);
		Wheel->SetupAttachment(VehicleRoot);
		Wheel->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		Wheel->SetStaticMesh(ConstructorHelpers::FObjectFinder<UStaticMesh>(AssetPath).Object);
		Wheel->SetRelativeLocation(Position);
	};
	setupWheel(WheelFL, TEXT("WheelFL"), TEXT("/Game/FS/Generated/Car/SM_Wheel_FL/StaticMeshes/SM_Wheel_FL.SM_Wheel_FL"), FVector(76.5f, -62.0f, HubHeight));
	setupWheel(WheelFR, TEXT("WheelFR"), TEXT("/Game/FS/Generated/Car/SM_Wheel_FR/StaticMeshes/SM_Wheel_FR.SM_Wheel_FR"), FVector(76.5f, 62.0f, HubHeight));
	setupWheel(WheelRL, TEXT("WheelRL"), TEXT("/Game/FS/Generated/Car/SM_Wheel_RL/StaticMeshes/SM_Wheel_RL.SM_Wheel_RL"), FVector(-76.5f, -60.0f, HubHeight));
	setupWheel(WheelRR, TEXT("WheelRR"), TEXT("/Game/FS/Generated/Car/SM_Wheel_RR/StaticMeshes/SM_Wheel_RR.SM_Wheel_RR"), FVector(-76.5f, 60.0f, HubHeight));

	// The mechanical parts from the team CAD (tools\cad_to_ue.py): suspension and the rest fixed to the body, uprights, calipers
	// and motors at each wheel centre turning with the steering, discs and hubs spinning with the wheel.
	const auto setupCad = [this](const TCHAR* Name, USceneComponent* Parent, const FVector& Position)
	{
		UStaticMeshComponent* Part = CreateDefaultSubobject<UStaticMeshComponent>(Name);
		Part->SetupAttachment(Parent);
		Part->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		Part->SetRelativeLocation(Position);
		const FString Asset = FString::Printf(TEXT("/Game/FS/Generated/Car/SM_%s/StaticMeshes/SM_%s.SM_%s"), Name, Name, Name);
		Part->SetStaticMesh(ConstructorHelpers::FObjectFinder<UStaticMesh>(*Asset).Object);
		return Part;
	};
	setupCad(TEXT("Cad_Body"), Body, FVector::ZeroVector);
	CadHubFL = setupCad(TEXT("Cad_Hub_FL"), VehicleRoot, WheelFL->GetRelativeLocation());
	CadHubFR = setupCad(TEXT("Cad_Hub_FR"), VehicleRoot, WheelFR->GetRelativeLocation());
	setupCad(TEXT("Cad_Hub_RL"), VehicleRoot, WheelRL->GetRelativeLocation());
	setupCad(TEXT("Cad_Hub_RR"), VehicleRoot, WheelRR->GetRelativeLocation());
	setupCad(TEXT("Cad_Spin_FL"), WheelFL, FVector::ZeroVector);
	setupCad(TEXT("Cad_Spin_FR"), WheelFR, FVector::ZeroVector);
	setupCad(TEXT("Cad_Spin_RL"), WheelRL, FVector::ZeroVector);
	setupCad(TEXT("Cad_Spin_RR"), WheelRR, FVector::ZeroVector);

	SteeringWheel = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SteeringWheel"));
	SteeringWheel->SetupAttachment(Body);
	SteeringWheel->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SteeringWheel->SetRelativeLocation(SteeringWheelPosition);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SteeringWheelAsset(
		TEXT("/Game/FS/Generated/Car/SM_Cad_SteeringWheel/StaticMeshes/SM_Cad_SteeringWheel.SM_Cad_SteeringWheel"));   // the team's wheel (tools\cad_to_ue.py)
	if (SteeringWheelAsset.Succeeded())
	{
		SteeringWheel->SetStaticMesh(SteeringWheelAsset.Object);
	}

	// the camera is placed in world space every frame (UpdateCamera)
	ChaseCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ChaseCamera"));
	ChaseCamera->SetupAttachment(VehicleRoot);
	ChaseCamera->SetUsingAbsoluteLocation(true);
	ChaseCamera->SetUsingAbsoluteRotation(true);
	ChaseCamera->FieldOfView = 90;
	ChaseCamera->PostProcessSettings.bOverride_MotionBlurAmount = true;
	ChaseCamera->PostProcessSettings.MotionBlurAmount = 0.0f;   // off: even a little smeared the spinning tyres, which looked faded and flickering
	FPostProcessSettings& Look = ChaseCamera->PostProcessSettings;
	Look.bOverride_BloomIntensity = Look.bOverride_AutoExposureBias = Look.bOverride_AutoExposureSpeedUp = Look.bOverride_AutoExposureSpeedDown = Look.bOverride_ColorSaturation = Look.bOverride_ColorContrast = true;
	Look.BloomIntensity = 0.35f;
	Look.AutoExposureBias = -0.15f;                               // the concrete was washing out
	Look.AutoExposureSpeedUp = Look.AutoExposureSpeedDown = 1.0f;   // slow, so the picture doesn't pump between sky and tarmac
	Look.ColorSaturation = FVector4(1.08, 1.08, 1.08, 1);
	Look.ColorContrast = FVector4(1.06, 1.06, 1.06, 1);

	SetActorEnableCollision(false);
}

void AFSLapVehiclePawn::BeginPlay()
{
	Super::BeginPlay();
	Track = HoMoLap::MakeFsgTrack();
	CityTrack = HoMoLap::MakeCityTrack();
	LoadBests();
	// shortcut: while another Unreal holds the map file it can't be re-saved without its old ground actor; this is harmless once it has none.
	for (TActorIterator<AStaticMeshActor> It(GetWorld()); It; ++It)
	{
		It->SetActorHiddenInGame(true);
	}
	SpawnScenery();
	MakeSounds();
	MakeTraffic();
	EnterMenu();
}

USceneComponent* AFSLapVehiclePawn::MakeGhost(AActor* Holder, const TCHAR* MaterialPath)
{
	USceneComponent* Ghost = NewObject<USceneComponent>(Holder);
	Ghost->SetupAttachment(Holder->GetRootComponent());
	Ghost->RegisterComponent();
	Holder->AddInstanceComponent(Ghost);
	UMaterialInterface* Material = LoadObject<UMaterialInterface>(nullptr, MaterialPath);
	for (UStaticMeshComponent* Source : { Body, WheelFL, WheelFR, WheelRL, WheelRR })
	{
		UStaticMeshComponent* Part = NewObject<UStaticMeshComponent>(Holder);
		Part->SetStaticMesh(Source->GetStaticMesh());
		Part->SetForceDisableNanite(true);   // Nanite can't draw translucent materials
		Part->SetCastShadow(false);
		Part->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		Part->SetRelativeLocation(Source->GetRelativeLocation());
		for (int32 Slot = 0; Slot < Part->GetNumMaterials(); ++Slot)
		{
			Part->SetMaterial(Slot, Material);
		}
		Part->SetupAttachment(Ghost);
		Part->RegisterComponent();
		Holder->AddInstanceComponent(Part);
	}
	Ghost->SetVisibility(false, true);
	return Ghost;
}

void AFSLapVehiclePawn::SpawnScenery()
{
	AActor* Holder = GetWorld()->SpawnActor<AActor>();
	USceneComponent* HolderRoot = NewObject<USceneComponent>(Holder);
	Holder->SetRootComponent(HolderRoot);
	HolderRoot->RegisterComponent();
	const auto MakeSet = [&](const TCHAR* Path)
	{
		UInstancedStaticMeshComponent* Set = NewObject<UInstancedStaticMeshComponent>(Holder);
		Set->SetStaticMesh(LoadObject<UStaticMesh>(nullptr, Path));
		Set->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		Set->SetupAttachment(HolderRoot);
		Set->RegisterComponent();
		Holder->AddInstanceComponent(Set);
		return Set;
	};
	UInstancedStaticMeshComponent* Blue = MakeSet(TEXT("/Game/FS/Generated/Track/cone_b/StaticMeshes/cone_b.cone_b"));
	UInstancedStaticMeshComponent* Yellow = MakeSet(TEXT("/Game/FS/Generated/Track/cone_y/StaticMeshes/cone_y.cone_y"));
	UInstancedStaticMeshComponent* Orange = MakeSet(TEXT("/Game/FS/Generated/Track/cone_o/StaticMeshes/cone_o.cone_o"));
	Cones.Reset(Track.cones.size());
	for (const HoMoLap::Cone& Cone : Track.cones)
	{
		UInstancedStaticMeshComponent* Set = Cone.col == 'b' ? Blue : (Cone.col == 'y' ? Yellow : Orange);
		const FVector Location(Cone.x * MetersToCentimeters, Cone.y * MetersToCentimeters, 0);
		const FTransform Upright(FRotator::ZeroRotator, Location, Cone.big ? FVector(1.25f, 1.25f, 1.55f) : FVector::OneVector);
		Cones.Add({ Set, Set->AddInstance(Upright, /*bWorldSpace=*/true), Upright, 0 });
	}

	const auto MakeMesh = [&](const TCHAR* Path)
	{
		UStaticMeshComponent* Mesh = NewObject<UStaticMeshComponent>(Holder);
		Mesh->SetStaticMesh(LoadObject<UStaticMesh>(nullptr, Path));
		Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		Mesh->SetupAttachment(HolderRoot);
		Mesh->RegisterComponent();
		Holder->AddInstanceComponent(Mesh);
		return Mesh;
	};
	Ground = MakeMesh(TEXT("/Game/FS/Generated/Track/ground/StaticMeshes/ground.ground"));
	Paddock = MakeMesh(TEXT("/Game/FS/Generated/Track/paddock/StaticMeshes/paddock.paddock"));
	// flat ground casts no shadow: its road and lines sit millimetres above the ground around them, and their thin shadows made them look lifted
	Ground->SetCastShadow(false);
	Paddock->SetCastShadow(false);
	const HoMoLap::Track& Made = CityTrack;
	UStaticMeshComponent* CityGround = MakeMesh(TEXT("/Game/FS/Generated/City/SM_CityBig/StaticMeshes/SM_CityBig.SM_CityBig"));
	CityGround->SetCastShadow(false);
	UInstancedStaticMeshComponent* CityBuildings = MakeSet(TEXT("/Game/FS/Generated/City/SM_CityBox/StaticMeshes/SM_CityBox.SM_CityBox"));
	UInstancedStaticMeshComponent* CityBarriers = MakeSet(TEXT("/Game/FS/Generated/City/SM_CityBarrier/StaticMeshes/SM_CityBarrier.SM_CityBarrier"));
	const auto AddBoxes = [](UInstancedStaticMeshComponent* Set, const std::vector<HoMoLap::CityBox>& Boxes)
	{
		for (const HoMoLap::CityBox& Box : Boxes)
		{
			Set->AddInstance(FTransform(FRotator::ZeroRotator, FVector(Box.x, Box.y, 0) * MetersToCentimeters, FVector(Box.hx * 2, Box.hy * 2, Box.h)), /*bWorldSpace=*/true);
		}
	};
	AddBoxes(CityBuildings, Made.buildings);
	AddBoxes(CityBarriers, Made.barriers);
	const TCHAR* const UnitBox = TEXT("/Game/FS/Generated/City/SM_CityPole/StaticMeshes/SM_CityPole.SM_CityPole");
	const TCHAR* const Sphere = TEXT("/Engine/BasicShapes/Sphere.Sphere");
	const auto Tinted = [&](const TCHAR* MeshPath, const TCHAR* MaterialPath)
	{
		UInstancedStaticMeshComponent* Set = MakeSet(MeshPath);
		Set->SetMaterial(0, LoadObject<UMaterialInterface>(nullptr, MaterialPath));
		return Set;
	};
	const auto AddPoles = [](UInstancedStaticMeshComponent* Set, const std::vector<HoMoLap::CityPole>& Poles, int32 Every = 1, int32 Offset = 0)
	{
		for (int32 i = Offset; i < static_cast<int32>(Poles.size()); i += Every)
		{
			const HoMoLap::CityPole& Pole = Poles[i];
			Set->AddInstance(FTransform(FRotator::ZeroRotator, FVector(Pole.x, Pole.y, Pole.z) * MetersToCentimeters, FVector(Pole.sx, Pole.sy, Pole.sz)), /*bWorldSpace=*/true);
		}
	};
	const auto AddTops = [](UInstancedStaticMeshComponent* Set, const std::vector<HoMoLap::CitySphere>& Tops)
	{
		for (const HoMoLap::CitySphere& Top : Tops)
		{
			Set->AddInstance(FTransform(FRotator::ZeroRotator, FVector(Top.x, Top.y, Top.z) * MetersToCentimeters, FVector(Top.r * 2)), /*bWorldSpace=*/true);
		}
	};
	const TCHAR* const Leaf = TEXT("/Game/FS/Generated/Materials/MI_CityLeaf.MI_CityLeaf");
	UInstancedStaticMeshComponent* CityPoles = MakeSet(UnitBox);
	AddPoles(CityPoles, Made.poles);
	UInstancedStaticMeshComponent* CityTrees = Tinted(Sphere, Leaf);
	AddTops(CityTrees, Made.treeTops);
	CityPoles->SetCullDistances(70000, 80000);   // thin poles and tree balls vanish into the haze long before this
	CityTrees->SetCullDistances(70000, 80000);
	CityParts = { CityGround, CityBuildings, CityBarriers, CityPoles, CityTrees };

	// the FSG paddock uses the same pieces with the materials that already exist
	UInstancedStaticMeshComponent* PaddockPoles = MakeSet(UnitBox);
	AddPoles(PaddockPoles, Track.poles);
	UInstancedStaticMeshComponent* PaddockTrees = Tinted(Sphere, Leaf);
	AddTops(PaddockTrees, Track.treeTops);
	UInstancedStaticMeshComponent* PaddockBoxes = Tinted(UnitBox, TEXT("/Game/FS/Generated/Materials/MI_White.MI_White"));
	AddBoxes(PaddockBoxes, Track.buildings);
	UInstancedStaticMeshComponent* PaddockFence = MakeSet(TEXT("/Game/FS/Generated/City/SM_CityBarrier/StaticMeshes/SM_CityBarrier.SM_CityBarrier"));
	AddBoxes(PaddockFence, Track.barriers);
	for (const TCHAR* Colour : { TEXT("MI_White.MI_White"), TEXT("MI_ConeBlue.MI_ConeBlue"), TEXT("MI_Barrier.MI_Barrier"), TEXT("MI_ConeYellow.MI_ConeYellow"), TEXT("MI_Black.MI_Black") })
	{
		TrafficSets.Add(Tinted(UnitBox, *(FString(TEXT("/Game/FS/Generated/Materials/")) + Colour)));   // visibility follows the active city in SpawnTrafficInstances
	}
	PaddockSets = { PaddockPoles, PaddockTrees, PaddockBoxes, PaddockFence };
	static const TCHAR* const CanopyColours[] = { TEXT("MI_Barrier.MI_Barrier"), TEXT("MI_ConeBlue.MI_ConeBlue"), TEXT("MI_ConeYellow.MI_ConeYellow"), TEXT("MI_White.MI_White") };
	for (int32 Colour = 0; Colour < UE_ARRAY_COUNT(CanopyColours); ++Colour)
	{
		UInstancedStaticMeshComponent* Canopies = Tinted(UnitBox, *(FString(TEXT("/Game/FS/Generated/Materials/")) + CanopyColours[Colour]));
		AddPoles(Canopies, Track.canopies, UE_ARRAY_COUNT(CanopyColours), Colour);
		PaddockSets.Add(Canopies);
	}

	// skid marks and smoke are pools of instances that get reused; an unused one is scaled to nothing
	const FTransform Unused(FRotator::ZeroRotator, FVector::ZeroVector, FVector::ZeroVector);
	Skids = MakeSet(TEXT("/Game/FS/Generated/Track/skid/StaticMeshes/skid.skid"));
	Skids->SetCastShadow(false);
	for (int32 i = 0; i < MaxSkids; ++i)
	{
		Skids->AddInstance(Unused, /*bWorldSpace=*/true);
	}
	Smoke = MakeSet(TEXT("/Engine/BasicShapes/Sphere.Sphere"));
	Smoke->SetNumCustomDataFloats(1);
	Smoke->SetMaterial(0, LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/FS/Generated/Materials/M_Smoke.M_Smoke")));
	Smoke->SetCastShadow(false);
	for (int32 i = 0; i < MaxPuffs; ++i)
	{
		Smoke->AddInstance(Unused, /*bWorldSpace=*/true);
	}
	Puffs.SetNum(MaxPuffs);

	GhostBest = MakeGhost(Holder, TEXT("/Game/FS/Generated/Materials/MI_GhostBest.MI_GhostBest"));
	GhostLts = MakeGhost(Holder, TEXT("/Game/FS/Generated/Materials/MI_GhostLts.MI_GhostLts"));
}

void AFSLapVehiclePawn::RestartSimulation(HoMoLap::Mode NewMode)
{
	RaceMode = NewMode;
	Simulation = MakeUnique<HoMoLap::Simulation>(ActiveTrack(), Setup, RaceMode);
	CurrentKey = MakeBestKey();
	WheelRotation = 0;
	FixedAccumulator = 0;
	SeenLaps = 0;
	BodyPitch = BodyRoll = BodyPitchRate = BodyRollRate = 0;
	bCameraSnap = true;
	ClearEffects();
	const bool bCity = RaceMode == HoMoLap::Mode::City;
	Ground->SetVisibility(!bCity);
	ChaseCamera->PostProcessSettings.AutoExposureBias = bCity ? -0.55f : -0.15f;   // the shaded street canyons pull the auto exposure up, which burns out the sunlit road
	ChaseCamera->PostProcessSettings.bOverride_IndirectLightingIntensity = true;
	ChaseCamera->PostProcessSettings.IndirectLightingIntensity = bCity ? 1.5f : 1.0f;
	for (TActorIterator<ADirectionalLight> Sun(GetWorld()); Sun; ++Sun)
	{
		Sun->SetActorRotation(FRotator(bCity ? -72 : -40, 35, 0));   // a high sun in the city, or the tall blocks leave whole streets in shade
	}
	Paddock->SetVisibility(RaceMode == HoMoLap::Mode::Free);
	for (USceneComponent* Part : CityParts)
	{
		Part->SetVisibility(bCity);
	}
	SpawnTrafficInstances();
	for (UInstancedStaticMeshComponent* Set : PaddockSets)
	{
		Set->SetVisibility(RaceMode == HoMoLap::Mode::Sim || RaceMode == HoMoLap::Mode::Arcade);
	}
	UpdateCones();
	UpdateVisuals();
}

void AFSLapVehiclePawn::StartRace(HoMoLap::Mode NewMode)
{
	RestartSimulation(NewMode);
	CountdownTime = 0;
	CountdownHold = FMath::FRandRange(0.5f, 1.3f);
	Flow = NewMode == HoMoLap::Mode::Free ? EFSLapFlow::Race : EFSLapFlow::Countdown;
}

void AFSLapVehiclePawn::EnterMenu()
{
	RestartSimulation(HoMoLap::Mode::Sim);
	Simulation->PlaceAt(Track.LEN * 0.35);
	Flow = EFSLapFlow::Menu;
	UpdateVisuals();
}

void AFSLapVehiclePawn::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	bWheelLive = SaveData && Wheel.Read(SaveData->WheelName, WheelAxes);
	WheelSteer = WheelThrottle = WheelBrake = 0;
	if (IsWheelSetUp())
	{
		const auto Axis = [this](int32 Which)
		{
			const float* Cal = &SaveData->WheelCal[Which * 3];   // axis, at rest, at full
			return FMath::Clamp((WheelAxes[FMath::Clamp(static_cast<int32>(Cal[0]), 0, WheelAxes.Num() - 1)] - Cal[1]) / (Cal[2] - Cal[1]), -1.0f, 1.0f);
		};
		const auto Pedal = [](float Value) { return Value > 0.03f ? Value : 0.0f; };
		WheelSteer = Axis(0);
		WheelThrottle = Pedal(Axis(1));
		WheelBrake = Pedal(Axis(2));
	}
	if (!Simulation)
	{
		return;
	}
	UpdateCones();
	UpdateGhosts();
	if (!bFrozen)
	{
		if (Flow == EFSLapFlow::Countdown)
		{
			CountdownTime += DeltaSeconds;
			if (CountdownTime >= 3.0f + CountdownHold)
			{
				Flow = EFSLapFlow::Race;
			}
		}
		else if (Flow == EFSLapFlow::Replay)
		{
			UpdateReplay(DeltaSeconds);
		}
		else if (Flow != EFSLapFlow::Paused && Flow != EFSLapFlow::Setup && Flow != EFSLapFlow::Wheel)
		{
			FixedAccumulator = FMath::Min(FixedAccumulator + DeltaSeconds, 0.1);
			while (FixedAccumulator >= HoMoLap::DT)
			{
				Simulate();
				FixedAccumulator -= HoMoLap::DT;
			}
			CaptureBest();
			UpdateVisuals();
			UpdateEffects(DeltaSeconds);
		}
	}
	UpdateCamera(DeltaSeconds);
	UpdateSound(DeltaSeconds);
	UpdateTraffic(DeltaSeconds);
}

void AFSLapVehiclePawn::MakeTraffic()
{
	// shortcut: box-shaped cars that follow fixed paths with no junction logic, until the game has real traffic and car models
	// rectangular loops of streets around the circuit
	const std::vector<double>& X = CityTrack.streetX;
	const std::vector<double>& Y = CityTrack.streetY;
	for (int32 Ring = 0; Ring <= 6; Ring += 2)   // every other street ring, all outside the circuit's part of the grid
	{
		const bool bReverse = Ring % 4 == 2;
		const double Lane = bReverse ? -4 : 4;   // the right-hand side of the direction of travel
		const double X0 = X[Ring] + Lane, Y0 = Y[Ring] + Lane, X1 = X[X.size() - 1 - Ring] - Lane, Y1 = Y[Y.size() - 1 - Ring] - Lane;
		FTrafficPath Path;
		Path.P = { { X0, Y0 }, { X1, Y0 }, { X1, Y1 }, { X0, Y1 }, { X0, Y0 } };
		if (bReverse)
		{
			Algo::Reverse(Path.P);
		}
		Path.S.Add(0);
		for (int32 i = 1; i < Path.P.Num(); ++i)
		{
			Path.S.Add(Path.S.Last() + FVector2D::Distance(Path.P[i - 1], Path.P[i]));
		}
		TrafficPaths.Add(MoveTemp(Path));
	}
	TrafficWalls = CityTrack.walls.size();
	for (int32 p = 0; p < TrafficPaths.Num(); ++p)
	{
		for (double Along = 40; Along < TrafficPaths[p].S.Last() - 40; Along += 220)
		{
			TrafficCars.Add({ p, Along, TrafficCars.Num() % 4 });
			CityTrack.walls.push_back({ 1e9, 1e9, 1e9, 1e9 });   // out of everyone's way until the first update
		}
	}
}

void AFSLapVehiclePawn::SpawnTrafficInstances()
{
	const bool bCity = RaceMode == HoMoLap::Mode::City;
	for (UInstancedStaticMeshComponent* Set : TrafficSets)
	{
		Set->ClearInstances();
		Set->SetVisibility(bCity);
	}
	if (!bCity)
	{
		return;
	}
	const FTransform Hidden(FRotator::ZeroRotator, FVector(0, 0, -50000));
	for (FTrafficCar& Car : TrafficCars)
	{
		Car.Body = TrafficSets[Car.Set]->AddInstance(Hidden, /*bWorldSpace=*/true);
		TrafficSets[4]->AddInstance(Hidden, /*bWorldSpace=*/true);
	}
}

bool AFSLapVehiclePawn::GetTrafficCar(FVector& Location) const
{
	if (RaceMode != HoMoLap::Mode::City || TrafficCars.IsEmpty())
	{
		return false;
	}
	FTransform Where;
	const FTrafficCar& Car = TrafficCars[0];
	TrafficSets[Car.Set]->GetInstanceTransform(Car.Body, Where, /*bWorldSpace=*/true);
	Location = Where.GetLocation();
	return true;
}

void AFSLapVehiclePawn::UpdateTraffic(float DeltaSeconds)
{
	if (RaceMode != HoMoLap::Mode::City)
	{
		return;
	}
	const bool bMove = Flow == EFSLapFlow::Race && !bFrozen;
	TArray<FTrafficCar>& Cars = TrafficCars;
	for (int32 i = 0; i < Cars.Num(); ++i)
	{
		FTrafficCar& Car = Cars[i];
		const FTrafficPath& Path = TrafficPaths[Car.Path];
		const double Len = Path.S.Last();
		if (bMove)
		{
			Car.Along = FMath::Fmod(Car.Along + DeltaSeconds * 12.0 + Len, Len);   // 43 km/h
		}
		const int32 Seg = FMath::Clamp(static_cast<int32>(Algo::UpperBound(Path.S, Car.Along)) - 1, 0, Path.P.Num() - 2);
		const double T = (Car.Along - Path.S[Seg]) / FMath::Max(Path.S[Seg + 1] - Path.S[Seg], 1e-9);
		const FVector2D Direction = (Path.P[Seg + 1] - Path.P[Seg]).GetSafeNormal();
		const FVector2D Pos = Path.P[Seg] + (Path.P[Seg + 1] - Path.P[Seg]) * T;
		Car.Yaw = FMath::FixedTurn(Car.Yaw, FMath::RadiansToDegrees(FMath::Atan2(Direction.Y, Direction.X)), DeltaSeconds * 180);
		const FRotator Rotation(0, Car.Yaw, 0);
		const FVector Where(Pos.X * MetersToCentimeters, Pos.Y * MetersToCentimeters, 12);
		TrafficSets[Car.Set]->UpdateInstanceTransform(Car.Body, FTransform(Rotation, Where, FVector(4.4, 1.8, 0.75)), /*bWorldSpace=*/true, /*bMarkRenderStateDirty=*/false);
		TrafficSets[4]->UpdateInstanceTransform(i, FTransform(Rotation, Where + FVector(0, 0, 75), FVector(2.2, 1.6, 0.55)), /*bWorldSpace=*/true, /*bMarkRenderStateDirty=*/false);
		const double C = FMath::Abs(FMath::Cos(FMath::DegreesToRadians(Car.Yaw))), Sn = FMath::Abs(FMath::Sin(FMath::DegreesToRadians(Car.Yaw)));
		const double HalfX = C * 2.2 + Sn * 0.9, HalfY = Sn * 2.2 + C * 0.9;   // the drawn box's footprint, whatever the heading
		CityTrack.walls[TrafficWalls + i] = { Pos.X - HalfX, Pos.Y - HalfY, Pos.X + HalfX, Pos.Y + HalfY };
	}
	for (UInstancedStaticMeshComponent* Set : TrafficSets)
	{
		Set->MarkRenderStateDirty();
	}
}

void AFSLapVehiclePawn::MakeSounds()
{
	const auto Load = [](const TCHAR* Asset) { return LoadObject<USoundWave>(nullptr, *FString::Printf(TEXT("/Game/FS/Generated/Sound/%s.%s"), Asset, Asset)); };
	const auto Loop = [this, Load](const TCHAR* Asset)
	{
		UAudioComponent* Sound = NewObject<UAudioComponent>(this);
		Sound->SetSound(Load(Asset));
		Sound->bAllowSpatialization = false;
		Sound->SetVolumeMultiplier(0);
		Sound->RegisterComponent();
		Sound->Play();
		return Sound;
	};
	for (const FMotorLoop& Motor : MotorLoops)
	{
		MotorSounds.Add(Loop(Motor.Asset));
	}
	TyreSound = Loop(TEXT("A_Tyre"));
	WindSound = Loop(TEXT("A_Wind"));
	GrassSound = Loop(TEXT("A_Grass"));
	ConeHitSound = Load(TEXT("A_ConeHit"));
	BeepSound = Load(TEXT("A_Beep"));
}

void AFSLapVehiclePawn::UpdateSound(float DeltaSeconds)
{
	const HoMoLap::Car& Car = Simulation->c;
	const bool bReplay = Flow == EFSLapFlow::Replay;
	const bool bDriving = bReplay || (Flow == EFSLapFlow::Race && !bFrozen);
	const float Speed = bReplay ? ReplaySpeed : FMath::Abs(Car.vx);
	const double Top = Simulation->C.vMotor * (Simulation->arcade ? 1.45 : 1.0);   // arcade: a taller top speed, the motor still ends at its limit
	const float MotorShare = FMath::Min(static_cast<float>(Speed / Top), 1.0f);
	MotorTone = MotorShare * static_cast<float>(Setup.nmax) * MotorToneOrder / 60;

	// the loudness model of the web version, set so a lap peaks near a third of full scale (tools\check_sound.py); a replay has no recorded power
	const float Power = bReplay ? 0.5f : FMath::Max(FMath::Clamp(static_cast<float>(Car.power / (Simulation->C.Pmax * (Simulation->arcade ? 1.75 : 1.0))), 0.0f, 1.0f), static_cast<float>(Car.brk) * 0.45f);   // the brake term: regen makes the motors whine too
	const float Target = bDriving ? 2.1f * (0.18f + 0.62f * Power + 0.2f * MotorShare) * FMath::Min(MotorTone / 320.0f, 1.0f) : 0.0f;   // 320 Hz: the lowest pitch the engine can play the low loop at
	SoundLevel += (Target - SoundLevel) * FMath::Min(DeltaSeconds * 12, 1.0f);

	// equal-power crossfades; the loops hold the same tones, so they join without a change of pitch
	const float ToMid = FMath::SmoothStep(950.0f, 1350.0f, MotorTone) * HALF_PI, ToHigh = FMath::SmoothStep(2700.0f, 3400.0f, MotorTone) * HALF_PI;
	const float Mix[] = { FMath::Cos(ToMid), FMath::Sin(ToMid) * FMath::Cos(ToHigh), FMath::Sin(ToHigh) };
	for (int32 i = 0; i < MotorSounds.Num(); ++i)
	{
		MotorSounds[i]->SetVolumeMultiplier(SoundLevel * Mix[i]);
		MotorSounds[i]->SetPitchMultiplier(FMath::Clamp(MotorTone / MotorLoops[i].ToneHz, 0.4f, 2.0f));
	}

	const bool bSqueal = bDriving && !bReplay && (Car.slipF || Car.slipR) && !Car.grass;
	SoundSqueal += ((bSqueal ? 1.0f : 0.0f) - SoundSqueal) * FMath::Min(DeltaSeconds * 10, 1.0f);
	TyreSound->SetVolumeMultiplier(1.35f * SoundSqueal * FMath::Min(Speed / 6.0f, 1.0f));

	const float Wind = bDriving ? FMath::Min(Speed / 40.0f, 1.0f) : 0.0f;
	WindSound->SetVolumeMultiplier(0.9f * Wind * Wind);
	WindSound->SetPitchMultiplier(0.7f + 0.6f * Wind);
	SoundGrass += ((bDriving && !bReplay && Car.grass ? 1.0f : 0.0f) - SoundGrass) * FMath::Min(DeltaSeconds * 10, 1.0f);
	GrassSound->SetVolumeMultiplier(1.6f * SoundGrass * FMath::Min(Speed / 15.0f, 1.0f));

	const int32 Lights = Flow == EFSLapFlow::Countdown ? GetLights() : 0;
	if (Lights > HeardLights)
	{
		UGameplayStatics::PlaySound2D(this, BeepSound, 0.5f);
	}
	else if (HeardLights > 0 && Flow == EFSLapFlow::Race)
	{
		UGameplayStatics::PlaySound2D(this, BeepSound, 0.7f, 1.5f);   // lights out
	}
	HeardLights = Lights;
}

void AFSLapVehiclePawn::ClearEffects()
{
	const FTransform Unused(FRotator::ZeroRotator, FVector::ZeroVector, FVector::ZeroVector);
	for (int32 i = 0; i < MaxSkids; ++i)
	{
		Skids->UpdateInstanceTransform(i, Unused, /*bWorldSpace=*/true, /*bMarkRenderStateDirty=*/false);
	}
	Skids->MarkRenderStateDirty();
	for (FPuff& Puff : Puffs)
	{
		Puff.Life = 0;
	}
	for (int32 i = 0; i < 4; ++i)
	{
		bWheelSkidding[i] = false;
	}
	SkidHead = PuffHead = 0;
	bPuffsActive = true;   // one more pass hides the last of them
}

void AFSLapVehiclePawn::UpdateEffects(float DeltaSeconds)
{
	const HoMoLap::Car& Car = Simulation->c;
	const bool bFast = FMath::Abs(Car.vx) > 3 && !Car.grass;
	const double Ch = FMath::Cos(Car.h), Sh = FMath::Sin(Car.h);
	static const double WheelPositions[4][2] = { { 0.765, -0.62 }, { 0.765, 0.62 }, { -0.765, -0.60 }, { -0.765, 0.60 } };
	// the height of the surface a mark lies on, in cm: roads sit above the ground around them (tools\track_to_ue.py: road 1 cm,
	// apron 0, free-roam paddock -2 cm; tools\city_to_ue.py: circuit asphalt 4 cm, streets 0). A mark at one fixed height floated.
	const HoMoLap::Track& OnTrack = ActiveTrack();
	const auto SurfaceCm = [this, &OnTrack](const FVector2D& P)
	{
		const bool bCity = RaceMode == HoMoLap::Mode::City;
		const bool bRoad = FMath::Abs(OnTrack.NearestPoint(P.X, P.Y).lat) < OnTrack.HALF + (bCity ? 0.0 : 0.3);
		if (bCity)
		{
			return bRoad ? 4.0 : 0.0;
		}
		const double* A = OnTrack.APRON;
		const bool bOnTrackGround = P.X > A[0] && P.X < A[2] && P.Y > A[1] && P.Y < A[3];
		return bRoad ? 1.0 : (bOnTrackGround || RaceMode != HoMoLap::Mode::Free ? 0.0 : -2.0);
	};
	bool bNewSkid = false;
	for (int32 W = 0; W < 4; ++W)
	{
		if (!bFast || !(W < 2 ? Car.slipF : Car.slipR))
		{
			bWheelSkidding[W] = false;
			continue;
		}
		const FVector2D Point(Car.x + WheelPositions[W][0] * Ch - WheelPositions[W][1] * Sh, Car.y + WheelPositions[W][0] * Sh + WheelPositions[W][1] * Ch);
		if (bWheelSkidding[W])
		{
			const FVector2D Step = Point - LastWheel[W];
			const double Length = Step.Size();
			if (Length > 0.02)
			{
				const FVector2D Middle = (Point + LastWheel[W]) * 0.5;
				const FTransform Mark(FRotator(0, FMath::RadiansToDegrees(FMath::Atan2(Step.Y, Step.X)), 0), FVector(Middle.X * MetersToCentimeters, Middle.Y * MetersToCentimeters, SurfaceCm(Middle) + 0.4), FVector(Length + 0.06, 0.2, 1));
				Skids->UpdateInstanceTransform(SkidHead, Mark, /*bWorldSpace=*/true, /*bMarkRenderStateDirty=*/false);
				SkidHead = (SkidHead + 1) % MaxSkids;
				bNewSkid = true;
			}
		}
		LastWheel[W] = Point;
		bWheelSkidding[W] = true;
		if (W >= 2 && FMath::FRand() < DeltaSeconds * 18)
		{
			Puffs[PuffHead] = { Point, 1.0f, 0.3f };
			PuffHead = (PuffHead + 1) % MaxPuffs;
			bPuffsActive = true;
		}
	}
	if (bNewSkid)
	{
		Skids->MarkRenderStateDirty();
	}
	if (!bPuffsActive)
	{
		return;
	}
	bool bAny = false;
	for (int32 i = 0; i < MaxPuffs; ++i)
	{
		FPuff& Puff = Puffs[i];
		Puff.Life = FMath::Max(0.0f, Puff.Life - DeltaSeconds * 1.1f);
		Puff.Radius += DeltaSeconds * 1.3f;
		bAny |= Puff.Life > 0;
		const FVector Location(Puff.Position.X * MetersToCentimeters, Puff.Position.Y * MetersToCentimeters, 25 + (1 - Puff.Life) * 60);
		Smoke->UpdateInstanceTransform(i, FTransform(FRotator::ZeroRotator, Location, FVector(Puff.Life > 0 ? Puff.Radius * 2 : 0)), /*bWorldSpace=*/true, /*bMarkRenderStateDirty=*/false);
		Smoke->SetCustomDataValue(i, 0, Puff.Life * 0.2f, /*bMarkRenderStateDirty=*/false);
	}
	Smoke->MarkRenderStateDirty();
	bPuffsActive = bAny;
}

int32 AFSLapVehiclePawn::GetSetupRowCount() const
{
	return 3 + UE_ARRAY_COUNT(SetupFields);   // the aero package, the fields, the steering wheel, driver assist
}

FString AFSLapVehiclePawn::GetSetupRowText(int32 Row) const
{
	if (Row == 0)
	{
		return FString::Printf(TEXT("Aero package: %s"), PresetIndex < 0 ? TEXT("custom") : AeroPresets[PresetIndex].Name);
	}
	if (Row == GetSetupRowCount() - 1)
	{
		return FString::Printf(TEXT("Driver assist:  %s"), bDriverAssist ? TEXT("On") : TEXT("Off"));
	}
	if (Row == GetSetupRowCount() - 2)
	{
		return !bWheelLive ? TEXT("Steering wheel:  none found") : FString::Printf(TEXT("Steering wheel:  %s, %s"), *Wheel.Name, IsWheelSetUp() ? TEXT("set up") : TEXT("not set up (RIGHT)"));
	}
	const FSetupField& Field = SetupFields[Row - 1];
	return FString::Printf(TEXT("%s:  %.*f %s"), Field.Label, Field.Decimals, Setup.*Field.Value * Field.Factor, Field.Unit);
}

void AFSLapVehiclePawn::ChangeSetup(int32 Direction)
{
	if (SetupRow == GetSetupRowCount() - 1)
	{
		bDriverAssist = !bDriverAssist;
	}
	else if (SetupRow == GetSetupRowCount() - 2)
	{
		Flow = EFSLapFlow::Wheel;
		WheelStep = 0;
		WheelMessage.Empty();
	}
	else if (SetupRow == 0)
	{
		const int32 Count = UE_ARRAY_COUNT(AeroPresets);
		PresetIndex = PresetIndex < 0 ? 0 : (PresetIndex + Direction + Count) % Count;
		Setup.ClA = AeroPresets[PresetIndex].ClA;
		Setup.CdA = AeroPresets[PresetIndex].CdA;
		Setup.bal = AeroPresets[PresetIndex].Bal;
	}
	else
	{
		const FSetupField& Field = SetupFields[SetupRow - 1];
		// snap to the grid of steps, so repeated presses can't drift
		Setup.*Field.Value = FMath::Clamp(FMath::RoundToDouble(Setup.*Field.Value / Field.Step + Direction) * Field.Step, Field.Min, Field.Max);
		if (SetupRow <= 3)
		{
			PresetIndex = -1;   // the three aero rows no longer match a package
		}
	}
}

void AFSLapVehiclePawn::MenuLeft()
{
	if (Flow == EFSLapFlow::Setup)
	{
		ChangeSetup(-1);
	}
}

void AFSLapVehiclePawn::MenuRight()
{
	if (Flow == EFSLapFlow::Setup)
	{
		ChangeSetup(1);
	}
}

void AFSLapVehiclePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);
	PlayerInputComponent->BindAxis(TEXT("Throttle"), this, &AFSLapVehiclePawn::Throttle);
	PlayerInputComponent->BindAxis(TEXT("Brake"), this, &AFSLapVehiclePawn::Brake);
	PlayerInputComponent->BindAxis(TEXT("Steer"), this, &AFSLapVehiclePawn::Steer);
	PlayerInputComponent->BindAction(TEXT("Handbrake"), IE_Pressed, this, &AFSLapVehiclePawn::HandbrakePressed);
	PlayerInputComponent->BindAction(TEXT("Handbrake"), IE_Released, this, &AFSLapVehiclePawn::HandbrakeReleased);
	PlayerInputComponent->BindAction(TEXT("ResetCar"), IE_Pressed, this, &AFSLapVehiclePawn::ResetPressed);
	PlayerInputComponent->BindAction(TEXT("CameraCycle"), IE_Pressed, this, &AFSLapVehiclePawn::CycleCamera);
	PlayerInputComponent->BindAction(TEXT("MenuUp"), IE_Pressed, this, &AFSLapVehiclePawn::MenuUp);
	PlayerInputComponent->BindAction(TEXT("MenuDown"), IE_Pressed, this, &AFSLapVehiclePawn::MenuDown);
	PlayerInputComponent->BindAction(TEXT("MenuLeft"), IE_Pressed, this, &AFSLapVehiclePawn::MenuLeft);
	PlayerInputComponent->BindAction(TEXT("MenuRight"), IE_Pressed, this, &AFSLapVehiclePawn::MenuRight);
	PlayerInputComponent->BindAction(TEXT("Confirm"), IE_Pressed, this, &AFSLapVehiclePawn::Confirm);
	PlayerInputComponent->BindAction(TEXT("Pause"), IE_Pressed, this, &AFSLapVehiclePawn::TogglePause);
	PlayerInputComponent->BindAction(TEXT("BackToMenu"), IE_Pressed, this, &AFSLapVehiclePawn::BackToMenu);
	PlayerInputComponent->BindAction(TEXT("Replay"), IE_Pressed, this, &AFSLapVehiclePawn::ReplayPressed);
}

void AFSLapVehiclePawn::Throttle(float Value)
{
	ThrottleInput = FMath::Clamp(Value, 0.0f, 1.0f);
}

void AFSLapVehiclePawn::Brake(float Value)
{
	BrakeInput = FMath::Clamp(Value, 0.0f, 1.0f);
}

void AFSLapVehiclePawn::Steer(float Value)
{
	SteerInput = FMath::Clamp(Value, -1.0f, 1.0f);
}

void AFSLapVehiclePawn::HandbrakePressed()
{
	Handbrake = true;
}

void AFSLapVehiclePawn::HandbrakeReleased()
{
	Handbrake = false;
}

void AFSLapVehiclePawn::ResetPressed()
{
	if (Flow == EFSLapFlow::Race)
	{
		Simulation->BackOnTrack();
		UpdateVisuals();
	}
	else if (Flow == EFSLapFlow::Paused)
	{
		StartRace(RaceMode);
	}
	else if (Flow == EFSLapFlow::Setup)
	{
		Setup = HoMoLap::Setup{};
		bDriverAssist = true;
		PresetIndex = 0;
	}
}

void AFSLapVehiclePawn::MenuUp()
{
	const int32 MenuRows = MenuSetupRow + 1;
	if (Flow == EFSLapFlow::Menu)
	{
		MenuIndex = (MenuIndex + MenuRows - 1) % MenuRows;
	}
	else if (Flow == EFSLapFlow::Setup)
	{
		SetupRow = (SetupRow + GetSetupRowCount() - 1) % GetSetupRowCount();
	}
}

void AFSLapVehiclePawn::MenuDown()
{
	const int32 MenuRows = MenuSetupRow + 1;
	if (Flow == EFSLapFlow::Menu)
	{
		MenuIndex = (MenuIndex + 1) % MenuRows;
	}
	else if (Flow == EFSLapFlow::Setup)
	{
		SetupRow = (SetupRow + 1) % GetSetupRowCount();
	}
}

void AFSLapVehiclePawn::ReplayPressed()
{
	if (Flow == EFSLapFlow::Paused && CanReplay())
	{
		StartReplay();
	}
}

void AFSLapVehiclePawn::StartReplay()
{
	ReplayReturn = Flow;
	ReplayTime = 0;
	Flow = EFSLapFlow::Replay;
	for (USceneComponent* Ghost : { GhostBest, GhostLts })
	{
		Ghost->SetVisibility(false, true);
	}
}

void AFSLapVehiclePawn::StopReplay()
{
	Flow = ReplayReturn;
	UpdateVisuals();   // back to where the car really is
}

void AFSLapVehiclePawn::UpdateReplay(float DeltaSeconds)
{
	const std::vector<HoMoLap::GhostPoint>& Points = CurrentBest().Ghost;
	if (Points.size() < 2)
	{
		StopReplay();
		return;
	}
	ReplayTime += DeltaSeconds;
	if (ReplayTime >= Points.back().t)
	{
		ReplayTime = 0;   // it loops until you leave
	}
	const auto Next = std::upper_bound(Points.begin(), Points.end(), ReplayTime, [](double Value, const HoMoLap::GhostPoint& Point) { return Value < Point.t; });
	const size_t Index = FMath::Clamp<size_t>(Next - Points.begin(), 1, Points.size() - 1);
	const HoMoLap::GhostPoint& A = Points[Index - 1];
	const HoMoLap::GhostPoint& B = Points[Index];
	const double Step = FMath::Max(B.t - A.t, 1e-3), F = FMath::Clamp((ReplayTime - A.t) / Step, 0.0, 1.0);
	const double YawRate = FMath::UnwindRadians(B.h - A.h) / Step, Speed = FMath::Sqrt(FMath::Square(B.x - A.x) + FMath::Square(B.y - A.y)) / Step;
	ReplaySpeed = Speed;
	SetActorLocationAndRotation(FVector(A.x + (B.x - A.x) * F, A.y + (B.y - A.y) * F, 0) * MetersToCentimeters,
		FRotator(0, FMath::RadiansToDegrees(A.h + FMath::UnwindRadians(B.h - A.h) * F), 0));

	// steering and body roll are not recorded: they follow from the yaw rate
	const float Steer = FMath::RadiansToDegrees(static_cast<float>(FMath::Atan(Simulation->C.L * YawRate / FMath::Max(Speed, 2.0))));
	WheelRotation = FMath::Fmod(WheelRotation - FMath::RadiansToDegrees(static_cast<float>(Speed * DeltaSeconds / Simulation->C.R)), 360.0f);
	WheelFL->SetRelativeRotation(FRotator(WheelRotation, Steer, 0));
	WheelFR->SetRelativeRotation(FRotator(WheelRotation, Steer, 0));
	CadHubFL->SetRelativeRotation(FRotator(0, Steer, 0));
	CadHubFR->SetRelativeRotation(FRotator(0, Steer, 0));
	WheelRL->SetRelativeRotation(FRotator(WheelRotation, 0, 0));
	WheelRR->SetRelativeRotation(FRotator(WheelRotation, 0, 0));
	SteeringWheel->SetRelativeRotation(FRotator(SteeringWheelTilt, 0, FMath::Clamp(Steer / 15.0f, -1.0f, 1.0f) * SteeringWheelTravel));
	const float RollTarget = FMath::RadiansToDegrees(FMath::Clamp(-static_cast<float>(YawRate * Speed) * 0.0016f, -0.06f, 0.06f));
	BodyRollRate += (140.0f * (RollTarget - BodyRoll) - 13.0f * BodyRollRate) * FMath::Min(DeltaSeconds, 0.05f);
	BodyRoll += BodyRollRate * FMath::Min(DeltaSeconds, 0.05f);
	Body->SetRelativeRotation(FRotator(0, 0, BodyRoll));
}

void AFSLapVehiclePawn::Confirm()
{
	if (Flow == EFSLapFlow::Replay)
	{
		StopReplay();
		return;
	}
	if (Flow == EFSLapFlow::Menu)
	{
		if (MenuIndex == MenuSetupRow)
		{
			Flow = EFSLapFlow::Setup;
		}
		else
		{
			StartRace(MenuModes[MenuIndex]);
		}
	}
	else if (Flow == EFSLapFlow::Paused)
	{
		Flow = ResumeTo;
	}
	else if (Flow == EFSLapFlow::Setup)
	{
		EnterMenu();
	}
	else if (Flow == EFSLapFlow::Wheel)
	{
		WheelNext();
	}
}

bool AFSLapVehiclePawn::IsWheelSetUp() const
{
	return bWheelLive && SaveData->WheelCal.Num() == 9 && Wheel.Name == SaveData->WheelName;
}

// The same steps as HoMo-LAP's wheel setup: at rest, then whichever axis moved most is steering, throttle, brake.
void AFSLapVehiclePawn::WheelNext()
{
	WheelMessage.Empty();
	if (!bWheelLive)
	{
		return;
	}
	if (WheelStep == 0)
	{
		WheelRest = WheelAxes;
		WheelNew.Reset();
		WheelStep = 1;
		return;
	}
	int32 Moved = 0;
	for (int32 i = 1; i < WheelAxes.Num(); ++i)
	{
		Moved = FMath::Abs(WheelAxes[i] - WheelRest[i]) > FMath::Abs(WheelAxes[Moved] - WheelRest[Moved]) ? i : Moved;
	}
	if (FMath::Abs(WheelAxes[Moved] - WheelRest[Moved]) < 0.2f)
	{
		WheelMessage = TEXT("Nothing moved enough. Try again.");
		return;
	}
	WheelNew.Append({ static_cast<float>(Moved), WheelRest[Moved], WheelAxes[Moved] });
	if (++WheelStep < 4)
	{
		return;
	}
	SaveData->WheelName = Wheel.Name;
	SaveData->WheelCal = WheelNew;
	UGameplayStatics::SaveGameToSlot(SaveData, SaveSlot, 0);
	WheelStep = 0;
	WheelMessage = TEXT("Saved. Check the line below, then ESC.");
}

FString AFSLapVehiclePawn::GetWheelText() const
{
	if (!bWheelLive)
	{
		return TEXT("No steering wheel found. Plug it in; it is\nfound within 2 s. The game reads DirectInput\nwheels such as the Moza R3.");
	}
	static const TCHAR* const Steps[] = {
		TEXT("Hands off the wheel and feet off the pedals,\nthen press ENTER."),
		TEXT("Turn the wheel to the right as far as you\nwant full steering lock, hold it there\nand press ENTER."),
		TEXT("Press the throttle pedal fully, hold it\nand press ENTER."),
		TEXT("Press the brake pedal fully, hold it\nand press ENTER.") };
	FString Text = Wheel.Name + TEXT("\n\n") + (WheelMessage.IsEmpty() ? Steps[WheelStep] : *WheelMessage) + TEXT("\n\n");
	for (int32 i = 0; i < WheelAxes.Num(); ++i)
	{
		Text += FString::Printf(TEXT("axis %d   %+.2f\n"), i, WheelAxes[i]);
	}
	if (IsWheelSetUp())
	{
		Text += FString::Printf(TEXT("\nNow: steer %+.2f   throttle %.2f   brake %.2f"), WheelSteer, WheelThrottle, WheelBrake);
	}
	return Text;
}

void AFSLapVehiclePawn::TogglePause()
{
	if (Flow == EFSLapFlow::Replay)
	{
		StopReplay();
	}
	else if (Flow == EFSLapFlow::Race || Flow == EFSLapFlow::Countdown)
	{
		ResumeTo = Flow;
		Flow = EFSLapFlow::Paused;
	}
	else if (Flow == EFSLapFlow::Paused)
	{
		Flow = ResumeTo;
	}
	else if (Flow == EFSLapFlow::Setup)
	{
		EnterMenu();
	}
	else if (Flow == EFSLapFlow::Wheel)
	{
		Flow = EFSLapFlow::Setup;
	}
}

void AFSLapVehiclePawn::BackToMenu()
{
	if (Flow == EFSLapFlow::Paused)
	{
		EnterMenu();
	}
}

void AFSLapVehiclePawn::CycleCamera()
{
	bOnboard = !bOnboard;
	bCameraSnap = true;
}

void AFSLapVehiclePawn::UpdateCamera(float DeltaSeconds)
{
	const HoMoLap::Car& Car = Simulation->c;
	const double Speed = std::hypot(Car.vx, Car.vy);
	float VerticalFov;
	if (Flow == EFSLapFlow::Replay)
	{
		// TV cameras: the nearest of ten roadside positions, zoomed so the car stays about the same size
		const HoMoLap::Track& OnTrack = ActiveTrack();
		const FVector Target = GetActorLocation() + FVector(0, 0, 50);
		FVector Nearest = FVector::ZeroVector;
		double NearestDistance = TNumericLimits<double>::Max();
		for (int32 i = 0; i < 10; ++i)
		{
			const HoMoLap::Pose Point = OnTrack.PoseAt(i * OnTrack.LEN / 10);
			const double Offset = (OnTrack.HALF + 3) * (i % 2 ? 1 : -1);
			const FVector Position(Point.x - FMath::Sin(Point.h) * Offset, Point.y + FMath::Cos(Point.h) * Offset, 2.4);
			const double Distance = FVector::Dist(Position * MetersToCentimeters, Target);
			if (Distance < NearestDistance)
			{
				NearestDistance = Distance;
				Nearest = Position * MetersToCentimeters;
			}
		}
		ChaseCamera->SetWorldLocationAndRotation(Nearest, (Target - Nearest).Rotation());
		ChaseCamera->SetFieldOfView(FMath::Clamp(2 * FMath::RadiansToDegrees(FMath::Atan(320.0 / FMath::Max(NearestDistance, 100.0))), 6.0, 70.0));
		bCameraSnap = true;   // the normal camera starts fresh after the replay
		return;
	}
	if (bOnboard)
	{
		// the driver's head: pushed around by the g-forces, and holding the horizon better than the car does
		const FVector Eye = DriverEye + FVector(FMath::Clamp(-Car.axp * 0.25, -3.0, 6.0), FMath::Clamp(-Car.ayp * 0.3, -6.0, 6.0), 0);
		const FRotator Look(BodyPitch * 0.5f - 5.0f, FMath::RadiansToDegrees(Car.h + Car.delta * 0.18), BodyRoll * 0.5f);
		ChaseCamera->SetWorldLocationAndRotation(Body->GetComponentTransform().TransformPosition(Eye), Look);
		VerticalFov = 60.0f + 3.0f * FMath::Clamp(static_cast<float>(Speed) / 34.0f, 0.0f, 1.0f);
	}
	else
	{
		// the web game's chase camera, with a slightly narrower view: it looks wider on a big window
		const bool bArcadeCamera = RaceMode != HoMoLap::Mode::Sim;
		const double Beta = std::atan2(Car.vy, FMath::Max(FMath::Abs(Car.vx), 1.5));
		const double YawTarget = Car.h + FMath::Clamp(Beta, -0.6, 0.6) * 0.6;
		CameraYaw = bCameraSnap ? YawTarget : CameraYaw + FMath::UnwindRadians(YawTarget - CameraYaw) * (1 - FMath::Exp(-5.0 * DeltaSeconds));
		const FVector Forward(FMath::Cos(CameraYaw), FMath::Sin(CameraYaw), 0);
		const double Distance = bArcadeCamera ? 5.0 + Speed * 0.05 : 5.4 + Speed * 0.045;
		const double Height = bArcadeCamera ? 1.75 + Speed * 0.012 : 1.9 + Speed * 0.012;
		const FVector CarPoint(Car.x * MetersToCentimeters, Car.y * MetersToCentimeters, 0);
		const FVector Wanted = CarPoint - Forward * Distance * MetersToCentimeters + FVector(0, 0, Height * MetersToCentimeters);
		CameraPosition = bCameraSnap ? Wanted : FMath::Lerp(CameraPosition, Wanted, 1 - FMath::Exp(-12.0 * DeltaSeconds));
		const FVector LookAt = CarPoint + Forward * 400 + FVector(0, 0, 55);
		ChaseCamera->SetWorldLocationAndRotation(CameraPosition, (LookAt - CameraPosition).Rotation());
		VerticalFov = bArcadeCamera ? 56.0f + 7.0f * FMath::Clamp(static_cast<float>(Speed) / 48.0f, 0.0f, 1.0f) : 54.0f + 5.0f * FMath::Clamp(static_cast<float>(Speed) / 34.0f, 0.0f, 1.0f);
	}
	const float WantedFov = HorizontalFov(VerticalFov);
	ChaseCamera->SetFieldOfView(bCameraSnap ? WantedFov : FMath::FInterpTo(ChaseCamera->FieldOfView, WantedFov, DeltaSeconds, 4.0f));
	bCameraSnap = false;
}

void AFSLapVehiclePawn::Simulate()
{
	if (bAutopilot || Flow == EFSLapFlow::Menu)
	{
		Simulation->Step(nullptr);
		return;
	}
	HoMoLap::Input Input;
	Input.steer = SteerInput != 0 ? SteerInput : WheelSteer;   // keys and pad first, the wheel when they are idle
	Input.thr = FMath::Max(ThrottleInput, WheelThrottle);
	Input.brk = FMath::Max(BrakeInput, WheelBrake);
	Input.hb = Handbrake;
	Input.human = true;
	Input.assist = bDriverAssist;
	Simulation->Step(&Input);
}

void AFSLapVehiclePawn::CaptureBest()
{
	if (Flow != EFSLapFlow::Race || Simulation->laps.size() == SeenLaps)
	{
		return;
	}
	SeenLaps = Simulation->laps.size();
	const HoMoLap::LapResult& Lap = Simulation->laps.back();
	FBest& Best = Bests.FindOrAdd(CurrentKey);
	if (Lap.valid && (Best.Time <= 0 || Lap.time < Best.Time))
	{
		Best.Time = Lap.time;
		Best.Penalty = Lap.pen;
		Best.Split1 = Lap.split1;
		Best.Split2 = Lap.split2;
		Best.Ghost = Simulation->lastRec;
		BuildProgress(Best, ActiveTrack());
		if (!bAutopilot)   // the acceptance test's AI laps stay out of your records
		{
			SaveBest(CurrentKey, Best);
		}
	}
}

const AFSLapVehiclePawn::FBest& AFSLapVehiclePawn::CurrentBest() const
{
	static const FBest None;
	const FBest* Best = Bests.Find(CurrentKey);
	return Best ? *Best : None;
}

FString AFSLapVehiclePawn::MakeBestKey() const
{
	FString Key = FString::FromInt(static_cast<int32>(RaceMode));
	for (const FSetupField& Field : SetupFields)
	{
		Key += FString::Printf(TEXT("_%.4f"), Setup.*Field.Value);
	}
	return bDriverAssist ? Key : Key + TEXT("_noassist");   // assisted laps keep the records they already have
}

void AFSLapVehiclePawn::LoadBests()
{
	SaveData = Cast<UFSLapSave>(UGameplayStatics::LoadGameFromSlot(SaveSlot, 0));
	if (!SaveData)
	{
		SaveData = Cast<UFSLapSave>(UGameplayStatics::CreateSaveGameObject(UFSLapSave::StaticClass()));
		return;
	}
	for (const TPair<FString, FFSLapSavedLap>& Saved : SaveData->Laps)
	{
		FBest Best;
		Best.Time = Saved.Value.Time;
		Best.Penalty = Saved.Value.Penalty;
		Best.Split1 = Saved.Value.Split1;
		Best.Split2 = Saved.Value.Split2;
		for (int32 i = 0; i + 3 < Saved.Value.Ghost.Num(); i += 4)
		{
			Best.Ghost.push_back({ Saved.Value.Ghost[i], Saved.Value.Ghost[i + 1], Saved.Value.Ghost[i + 2], Saved.Value.Ghost[i + 3] });
		}
		if (Saved.Key.Mid(1, 1) == TEXT("c"))
		{
			continue;   // a removed real city (c1 Erlangen, c2 Nuernberg): left in the save file, not shown
		}
		BuildProgress(Best, FCString::Atoi(*Saved.Key) == static_cast<int32>(HoMoLap::Mode::City) ? CityTrack : Track);
		Bests.Add(Saved.Key, MoveTemp(Best));
	}
}

void AFSLapVehiclePawn::BuildProgress(FBest& Best, const HoMoLap::Track& OnTrack) const
{
	Best.Progress.clear();
	Best.Progress.reserve(Best.Ghost.size());
	double Laps = 0, Last = 0;
	for (const HoMoLap::GhostPoint& Point : Best.Ghost)
	{
		const double S = OnTrack.NearestPoint(Point.x, Point.y).s;
		if (!Best.Progress.empty() && S < Last - OnTrack.LEN / 2)
		{
			Laps += OnTrack.LEN;   // past the line
		}
		Last = S;
		Best.Progress.push_back(FMath::Max(S + Laps, Best.Progress.empty() ? 0.0 : Best.Progress.back()));
	}
}

bool AFSLapVehiclePawn::GetGapToBest(double& Gap) const
{
	const FBest& Best = CurrentBest();
	if (Best.Time <= 0 || Best.Progress.size() < 2 || !Simulation->IsLapActive())
	{
		return false;
	}
	const double At = Simulation->LapProgress();
	const size_t Hi = FMath::Clamp<size_t>(std::upper_bound(Best.Progress.begin(), Best.Progress.end(), At) - Best.Progress.begin(), 1, Best.Progress.size() - 1);
	const double A = Best.Progress[Hi - 1], B = Best.Progress[Hi];
	const double F = B > A ? FMath::Clamp((At - A) / (B - A), 0.0, 1.0) : 0.0;
	const double GhostTime = Best.Ghost[Hi - 1].t + (Best.Ghost[Hi].t - Best.Ghost[Hi - 1].t) * F;
	Gap = Simulation->CurrentLapTime() + Simulation->CurrentLapPenalty() - GhostTime - Best.Penalty;
	return true;
}

void AFSLapVehiclePawn::SaveBest(const FString& Key, const FBest& Best)
{
	FFSLapSavedLap& Saved = SaveData->Laps.FindOrAdd(Key);
	Saved.Time = Best.Time;
	Saved.Penalty = Best.Penalty;
	Saved.Split1 = Best.Split1;
	Saved.Split2 = Best.Split2;
	Saved.Ghost.Reset(Best.Ghost.size() * 4);
	for (const HoMoLap::GhostPoint& Point : Best.Ghost)
	{
		Saved.Ghost.Append({ Point.t, Point.x, Point.y, Point.h });
	}
	UGameplayStatics::SaveGameToSlot(SaveData, SaveSlot, 0);
}

void AFSLapVehiclePawn::UpdateGhosts()
{
	const bool bLapOn = Flow == EFSLapFlow::Race && RaceMode != HoMoLap::Mode::Free && RaceMode != HoMoLap::Mode::City && Simulation->IsLapActive();   // the street canyons' exposure burns a ghost out to a white blob
	const double LapTime = Simulation->CurrentLapTime();
	const auto Place = [this](USceneComponent* Ghost, bool bShow, double X, double Y, double H)
	{
		const FVector At(X * MetersToCentimeters, Y * MetersToCentimeters, 0);
		const bool bVisible = bShow && !(bOnboard && FVector::Dist2D(At, GetActorLocation()) < 1500);   // from the cockpit, a ghost this close fills the whole view
		Ghost->SetVisibility(bVisible, true);
		if (bVisible)
		{
			Ghost->SetWorldLocationAndRotation(At, FRotator(0, FMath::RadiansToDegrees(H), 0));
		}
	};

	const bool bShowLts = bLapOn && !ActiveTrack().ltsT.empty() && LapTime <= ActiveTrack().ltsLap;
	const HoMoLap::Pose Lts = bShowLts ? ActiveTrack().PoseAt(ActiveTrack().LtsDistAt(LapTime)) : HoMoLap::Pose{ 0, 0, 0 };
	Place(GhostLts, bShowLts, Lts.x, Lts.y, Lts.h);

	const std::vector<HoMoLap::GhostPoint>& Points = CurrentBest().Ghost;
	const bool bShowBest = bLapOn && Points.size() >= 2 && LapTime <= Points.back().t;
	if (!bShowBest)
	{
		GhostBest->SetVisibility(false, true);
		return;
	}
	const auto Next = std::upper_bound(Points.begin(), Points.end(), LapTime, [](double Value, const HoMoLap::GhostPoint& Point) { return Value < Point.t; });
	const size_t Index = FMath::Clamp<size_t>(Next - Points.begin(), 1, Points.size() - 1);
	const HoMoLap::GhostPoint& A = Points[Index - 1];
	const HoMoLap::GhostPoint& B = Points[Index];
	const double F = FMath::Clamp((LapTime - A.t) / FMath::Max(B.t - A.t, 1e-6), 0.0, 1.0);
	Place(GhostBest, true, A.x + (B.x - A.x) * F, A.y + (B.y - A.y) * F, A.h + FMath::UnwindRadians(B.h - A.h) * F);
}

void AFSLapVehiclePawn::UpdateVisuals()
{
	const HoMoLap::Car& Car = Simulation->c;
	const float SteerAngle = FMath::RadiansToDegrees(static_cast<float>(Car.delta));
	WheelRotation = FMath::Fmod(WheelRotation - FMath::RadiansToDegrees(static_cast<float>(Car.vx * GetWorld()->GetDeltaSeconds() / Simulation->C.R)), 360.0f);
	// the web game's lean angles, on a sprung body (it dives, settles and rolls instead of following every force); larger in arcade
	const float DeltaSeconds = FMath::Min(GetWorld()->GetDeltaSeconds(), 0.05f);
	const float Gain = RaceMode == HoMoLap::Mode::Sim ? 1.0f : 1.7f;
	const float PitchTarget = FMath::RadiansToDegrees(FMath::Clamp(static_cast<float>(Car.axp) * 0.0012f * Gain, -0.04f * Gain, 0.04f * Gain));
	const float RollTarget = FMath::RadiansToDegrees(FMath::Clamp(-static_cast<float>(Car.ayp) * 0.0016f * Gain, -0.06f * Gain, 0.06f * Gain));
	const auto Spring = [DeltaSeconds](float& Value, float& Rate, float Target)
	{
		Rate += (140.0f * (Target - Value) - 13.0f * Rate) * DeltaSeconds;
		Value += Rate * DeltaSeconds;
	};
	Spring(BodyPitch, BodyPitchRate, PitchTarget);
	Spring(BodyRoll, BodyRollRate, RollTarget);
	SetActorLocationAndRotation(
		FVector(Car.x * MetersToCentimeters, Car.y * MetersToCentimeters, 0),
		FRotator(0, FMath::RadiansToDegrees(Car.h), 0));
	Body->SetRelativeRotation(FRotator(BodyPitch, 0, BodyRoll));
	SteeringWheel->SetRelativeRotation(FRotator(SteeringWheelTilt, 0, static_cast<float>(Car.st) * SteeringWheelTravel));
	WheelFL->SetRelativeRotation(FRotator(WheelRotation, SteerAngle, 0));
	WheelFR->SetRelativeRotation(FRotator(WheelRotation, SteerAngle, 0));
	CadHubFL->SetRelativeRotation(FRotator(0, SteerAngle, 0));
	CadHubFR->SetRelativeRotation(FRotator(0, SteerAngle, 0));
	WheelRL->SetRelativeRotation(FRotator(WheelRotation, 0, 0));
	WheelRR->SetRelativeRotation(FRotator(WheelRotation, 0, 0));
}

void AFSLapVehiclePawn::UpdateCones()
{
	const bool bFree = RaceMode == HoMoLap::Mode::Free, bCity = RaceMode == HoMoLap::Mode::City;
	for (int32 i = 0; i < Cones.Num(); ++i)
	{
		const char Wanted = (bCity || (Track.cones[i].freeOnly && !bFree)) ? 2 : (Simulation->coneDown[i] ? 1 : 0);
		if (Wanted == Cones[i].State)
		{
			continue;
		}
		if (Wanted == 1 && Cones[i].State == 0 && Flow == EFSLapFlow::Race)
		{
			UGameplayStatics::PlaySound2D(this, ConeHitSound);
		}
		FTransform Transform = Cones[i].Upright;
		if (Wanted == 1)
		{
			Transform.ConcatenateRotation(FRotator(0, 0, 90).Quaternion());
			Transform.AddToTranslation(FVector(0, 0, 11));
		}
		else if (Wanted == 2)
		{
			Transform.SetScale3D(FVector::ZeroVector);
		}
		Cones[i].Set->UpdateInstanceTransform(Cones[i].Instance, Transform, /*bWorldSpace=*/true, /*bMarkRenderStateDirty=*/true);
		Cones[i].State = Wanted;
	}
}
