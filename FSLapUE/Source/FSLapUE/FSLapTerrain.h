#pragma once

#include "CoreMinimal.h"
#include "HoMoLapCore.h"

class AActor;
class UMaterialInterface;
class UProceduralMeshComponent;
class USceneComponent;

// The city's ground, drawn from the physics' own height grid (HoMoLap::Track::terrain, 2 m) and land use: square chunks,
// each built at the detail its distance from the camera needs (every grid sample near it, every 2nd, 4th ... farther
// out), with a skirt round each chunk hiding the steps between two details. Near the camera it is the very surface the
// car drives on, so the car never floats or sinks; under the road meshes it is sunk a few centimetres (Track::land).
class FFSLapTerrain
{
public:
	void Init(AActor* Owner, USceneComponent* Parent, const HoMoLap::Track& Ground, UMaterialInterface* Material);
	void Update(const FVector& Viewer, bool bAll = false);   // cm; rebuilds the chunks whose detail changed, nearest first
	void SetVisible(bool bVisible);
	int32 CountTriangles() const;

	static constexpr int32 ChunkSamples = 128;   // grid steps per chunk side (256 m)

private:
	int32 WantedLevel(int32 Chunk, const FVector2D& Viewer) const;
	void Build(int32 Chunk, int32 Level);

	const HoMoLap::Track* T = nullptr;
	TArray<UProceduralMeshComponent*> Chunks;
	TArray<int32> Levels, Triangles;   // per chunk: the sample stride it was built with (1, 2, 4 ...), -1 not yet
	int32 ChunksX = 0, ChunksY = 0;
	bool bShown = false;
};
