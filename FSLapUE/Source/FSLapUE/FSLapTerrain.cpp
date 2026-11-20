#include "FSLapTerrain.h"

#include "GameFramework/Actor.h"
#include "Materials/MaterialInterface.h"
#include "ProceduralMeshComponent.h"

namespace
{
	constexpr double Cm = 100.0;
	// how many grid samples a chunk skips, by its distance (m) from the camera: every sample within 1 km, where the game
	// also draws the road meshes on it (FSLapVehiclePawn: RoadDrawDistance)
	struct FBand { double Distance; int32 Stride; };
	constexpr FBand Bands[] = { { 1000, 1 }, { 2000, 2 }, { 3500, 4 }, { 6000, 8 }, { 1e12, 16 } };
	constexpr int32 RebuildsPerUpdate = 6;
}

void FFSLapTerrain::Init(AActor* Owner, USceneComponent* Parent, const HoMoLap::Track& Ground, UMaterialInterface* Material)
{
	T = &Ground;
	ChunksX = FMath::DivideAndRoundUp(Ground.tnx - 1, ChunkSamples);
	ChunksY = FMath::DivideAndRoundUp(Ground.tny - 1, ChunkSamples);
	for (int32 c = 0; c < ChunksX * ChunksY; ++c)
	{
		UProceduralMeshComponent* Mesh = NewObject<UProceduralMeshComponent>(Owner);
		Mesh->SetupAttachment(Parent);
		Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		Mesh->SetRelativeLocation(FVector((Ground.tx0 + (c % ChunksX) * ChunkSamples * Ground.tstep) * Cm, (Ground.ty0 + (c / ChunksX) * ChunkSamples * Ground.tstep) * Cm, 0));
		Mesh->SetMaterial(0, Material);
		Mesh->SetVisibility(false);
		Mesh->RegisterComponent();
		Owner->AddInstanceComponent(Mesh);
		Chunks.Add(Mesh);
	}
	Levels.Init(-1, Chunks.Num());
	Triangles.Init(0, Chunks.Num());
}

int32 FFSLapTerrain::WantedLevel(int32 Chunk, const FVector2D& Viewer) const
{
	const double Size = ChunkSamples * T->tstep;
	const double X0 = T->tx0 + (Chunk % ChunksX) * Size, Y0 = T->ty0 + (Chunk / ChunksX) * Size;
	const double Dx = FMath::Max3(X0 - Viewer.X, 0.0, Viewer.X - (X0 + Size)), Dy = FMath::Max3(Y0 - Viewer.Y, 0.0, Viewer.Y - (Y0 + Size));
	const double D = FMath::Sqrt(Dx * Dx + Dy * Dy);
	for (const FBand& Band : Bands)
	{
		if (D < Band.Distance)
		{
			return Band.Stride;
		}
	}
	return Bands[UE_ARRAY_COUNT(Bands) - 1].Stride;
}

void FFSLapTerrain::Update(const FVector& Viewer, bool bAll)
{
	if (!T || !bShown)
	{
		return;
	}
	const FVector2D At(Viewer.X / Cm, Viewer.Y / Cm);
	TArray<TPair<double, int32>> Stale;   // (distance, chunk) of every chunk whose detail is wrong
	for (int32 c = 0; c < Chunks.Num(); ++c)
	{
		if (WantedLevel(c, At) != Levels[c])
		{
			const double Size = ChunkSamples * T->tstep;
			Stale.Add({ FVector2D::Distance(At, FVector2D(T->tx0 + (c % ChunksX + 0.5) * Size, T->ty0 + (c / ChunksX + 0.5) * Size)), c });
		}
	}
	Stale.Sort([](const TPair<double, int32>& A, const TPair<double, int32>& B) { return A.Key < B.Key; });
	for (int32 k = 0; k < Stale.Num() && (bAll || k < RebuildsPerUpdate); ++k)
	{
		Build(Stale[k].Value, WantedLevel(Stale[k].Value, At));
	}
}

void FFSLapTerrain::SetVisible(bool bVisible)
{
	bShown = bVisible;
	for (int32 c = 0; c < Chunks.Num(); ++c)
	{
		Chunks[c]->SetVisibility(bVisible && Levels[c] > 0);
	}
}

int32 FFSLapTerrain::CountTriangles() const
{
	int32 Sum = 0;
	for (const int32 Count : Triangles)
	{
		Sum += Count;
	}
	return Sum;
}

void FFSLapTerrain::Build(int32 Chunk, int32 Stride)
{
	const HoMoLap::Track& G = *T;
	const int32 Nx = G.tnx, Ny = G.tny;
	const int32 I0 = (Chunk % ChunksX) * ChunkSamples, J0 = (Chunk / ChunksX) * ChunkSamples;
	const int32 N = ChunkSamples / Stride;
	const double Step = G.tstep;
	float SinkDepth = 0;   // how far the ground is sunk under the road meshes, where Track::land says so
	for (size_t k = 4; k < G.palette.size(); k += 6)
	{
		SinkDepth = FMath::Max(SinkDepth, G.palette[k]);
	}
	const auto Height = [&G, Nx, Ny](int32 I, int32 J) { return G.terrain[static_cast<size_t>(FMath::Clamp(J, 0, Ny - 1)) * Nx + FMath::Clamp(I, 0, Nx - 1)]; };

	TArray<FVector> Vertices;
	TArray<FVector> Normals;
	TArray<FVector2D> Uvs;
	TArray<FColor> Colors;
	TArray<int32> Indices;
	const int32 Side = N + 1;
	Vertices.Reserve(Side * Side + 4 * Side);
	for (int32 B = 0; B <= N; ++B)
	{
		for (int32 A = 0; A <= N; ++A)
		{
			const int32 I = FMath::Min(I0 + A * Stride, Nx - 1), J = FMath::Min(J0 + B * Stride, Ny - 1);
			const uint8 Land = G.land.empty() ? 0 : G.land[static_cast<size_t>(J) * Nx + I];
			const double Z = Height(I, J) - ((Land & 128) ? SinkDepth : 0.0);
			Vertices.Add(FVector((I - I0) * Step * Cm, (J - J0) * Step * Cm, Z * Cm));
			const double Dzdx = (Height(I + Stride, J) - Height(I - Stride, J)) / (2 * Stride * Step);
			const double Dzdy = (Height(I, J + Stride) - Height(I, J - Stride)) / (2 * Stride * Step);
			Normals.Add(FVector(-Dzdx, -Dzdy, 1).GetSafeNormal());
			Uvs.Add(FVector2D((G.tx0 + I * Step) / 4.0, (G.ty0 + J * Step) / 4.0));
			// the land use's colour, averaged over the samples this vertex stands for, so far ground doesn't flicker
			FLinearColor Sum(0, 0, 0, 0);
			int32 Count = 0;
			const int32 Half = Stride / 2;
			for (int32 Jb = FMath::Max(J - Half, 0); Jb <= FMath::Min(J + Half - (Stride > 1 ? 1 : 0), Ny - 1); ++Jb)
			{
				for (int32 Ib = FMath::Max(I - Half, 0); Ib <= FMath::Min(I + Half - (Stride > 1 ? 1 : 0), Nx - 1); ++Ib)
				{
					const int32 Class = G.land.empty() ? 0 : (G.land[static_cast<size_t>(Jb) * Nx + Ib] & 15);
					if (static_cast<size_t>(Class * 6 + 3) < G.palette.size())
					{
						const float* P = &G.palette[Class * 6];
						Sum += FLinearColor(P[0], P[1], P[2], P[3]);
						++Count;
					}
				}
			}
			const FLinearColor Mean = Count ? Sum / static_cast<float>(Count) : FLinearColor(0.08f, 0.1f, 0.04f, 0);
			Colors.Add(Mean.ToFColor(false));   // linear values: the terrain material reads them as they are
		}
	}
	// front faces as Unreal's own box mesh builds its top (UKismetProceduralMeshLibrary::GenerateBoxMesh): (x0 y0, x0 y1, x1 y0) faces up
	for (int32 B = 0; B < N; ++B)
	{
		for (int32 A = 0; A < N; ++A)
		{
			const int32 V00 = B * Side + A, V10 = V00 + 1, V01 = V00 + Side, V11 = V01 + 1;
			Indices.Append({ V00, V01, V10, V10, V01, V11 });
		}
	}
	// the skirt: each border vertex again, lower, so a coarser neighbour's edge never shows a gap; drawn from both sides
	const double Drop = (0.5 + 0.5 * Stride * Step) * Cm;
	const auto Skirt = [&](int32 First, int32 Delta)
	{
		const int32 Base = Vertices.Num();
		for (int32 k = 0; k <= N; ++k)
		{
			const int32 V = First + k * Delta;
			const FVector Low = Vertices[V] - FVector(0, 0, Drop), Normal = Normals[V];   // copies: Add may move the arrays
			const FVector2D Uv = Uvs[V];
			const FColor Color = Colors[V];
			Vertices.Add(Low);
			Normals.Add(Normal);
			Uvs.Add(Uv);
			Colors.Add(Color);
		}
		for (int32 k = 0; k < N; ++k)
		{
			const int32 Top0 = First + k * Delta, Top1 = Top0 + Delta, Low0 = Base + k, Low1 = Low0 + 1;
			Indices.Append({ Top0, Top1, Low0, Top1, Low1, Low0, Top0, Low0, Top1, Top1, Low0, Low1 });
		}
	};
	Skirt(0, 1);                   // y = 0 edge
	Skirt(N * Side, 1);            // far y edge
	Skirt(0, Side);                // x = 0 edge
	Skirt(N, Side);                // far x edge

	UProceduralMeshComponent* Mesh = Chunks[Chunk];
	Mesh->CreateMeshSection(0, Vertices, Indices, Normals, Uvs, Colors, TArray<FProcMeshTangent>(), /*bCreateCollision=*/false);
	Mesh->SetCastShadow(Stride <= 2);   // far ground: no shadow-map work for detail nobody sees
	Mesh->SetVisibility(bShown);
	Levels[Chunk] = Stride;
	Triangles[Chunk] = Indices.Num() / 3;
}
