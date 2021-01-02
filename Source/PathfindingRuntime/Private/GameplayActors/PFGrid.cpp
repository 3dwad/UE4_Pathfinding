#include "PFGrid.h"
#include "PathfindingRuntime/PathfindingRuntime.h"
#include "PFObstacleMaster.h"

#include "DrawDebugHelpers.h"
#include "Components/BillboardComponent.h"
#include "Kismet/KismetSystemLibrary.h"

APFGrid::APFGrid()
{
	//PrimaryActorTick.bCanEverTick = true;

	SceneRoot = CreateDefaultSubobject<USceneComponent>("SceneRoot");
	RootComponent = SceneRoot;

	Billboard = CreateDefaultSubobject<UBillboardComponent>("Billboard");
	Billboard->SetupAttachment(GetRootComponent());
}

void APFGrid::BeginPlay()
{
	Super::BeginPlay();
}

void APFGrid::ConstructionScriptLogic()
{
	FlushPersistentDebugLines(GetWorld());
	GridWorldLocation = SceneRoot->GetComponentLocation();
	DrawDebugBox(GetWorld(), GridWorldLocation, FVector(GridSizeWorld.Y, GridSizeWorld.X, 5.f), GridBoxColor, false, 600.f, SDPG_World, 10);
	DrawDebugSphere(GetWorld(), GetGridBottomLeft(), 30.f, 5, FColor::Red, false, 600.f, SDPG_World, 4);
	GenerateMapDataFromWorld();
	DrawTile();
}

FVector APFGrid::GetGridBottomLeft() const
{
	return GridWorldLocation - (SceneRoot->GetRightVector() * GridSizeWorld.X) - (SceneRoot->GetForwardVector() * GridSizeWorld.Y);
}

void APFGrid::GetGridTileNumber(int32& OutGridTileNumberX, int32& OutGridTileNumberY) const
{
	OutGridTileNumberX = static_cast<int32>(FMath::RoundToZero((GridSizeWorld / TileSize).X));
	OutGridTileNumberY = static_cast<int32>(FMath::RoundToZero((GridSizeWorld / TileSize).Y));
}

void APFGrid::DrawTile()
{
	for (auto Element : Tiles)
	{
		FColor TileColor = FColor::Green;

		switch (Element.Value.ObstacleType)
		{
		case OT_Complicated:
			TileColor = FColor::Yellow;
			break;
		case OT_Difficult:
			TileColor = FColor::Orange;
			break;
		case OT_Impassable:
			TileColor = FColor::Red;
			break;
		case OT_None:
			TileColor = FColor::Silver;
			break;
		default: ;
		}

		FPlane GridPlane = FPlane(0.f, 0.f, 1.f, GridWorldLocation.Z);
		DrawDebugSolidPlane(GetWorld(), GridPlane, Element.Value.WorldLocation, TileSize - TileSizeMinus, TileColor, false, 600.f);
	}
}

bool APFGrid::SphereTileTrace(const FVector& TileLocation, const ECollisionChannel& InCollisionChannel, EObstacleType& OutObstacleType)
{
	const ETraceTypeQuery GroundTypeQuery = UEngineTypes::ConvertToTraceType(InCollisionChannel);
	FHitResult TraceResult;
	const bool bTraceSuccessfully = UKismetSystemLibrary::SphereTraceSingle
	(
		GetWorld(),
		TileLocation,
		TileLocation,
		TileSize - TileSizeMinus,
		GroundTypeQuery,
		false,
		TArray<AActor*>{},
		EDrawDebugTrace::ForDuration,
		TraceResult,
		true,
		FLinearColor::Red,
		FLinearColor::Green,
		20.f
	);

	OutObstacleType = GetObstacleType(TraceResult.Actor);
	return bTraceSuccessfully;
}

EObstacleType APFGrid::GetObstacleType(const TWeakObjectPtr<AActor>& InActor) const
{
	EObstacleType ObstacleTypeResult = OT_Normal;

	if (const auto ObstacleActor = Cast<APFObstacleMaster>(InActor))
		ObstacleTypeResult = ObstacleActor->ObstacleType;

	return ObstacleTypeResult;
}

void APFGrid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APFGrid::GenerateMapDataFromWorld()
{
	Tiles.Empty();

	int32 TilesByX = 0;
	int32 TilesByY = 0;
	GetGridTileNumber(TilesByX, TilesByY);

	for (int IndexByX = 0; IndexByX < TilesByX; ++IndexByX)
	{
		for (int IndexByY = 0; IndexByY < TilesByY; ++IndexByY)
		{
			FVector OffsetByY = SceneRoot->GetForwardVector() * (TileSize * 2 * IndexByY + TileSize);
			FVector OffsetByX = SceneRoot->GetRightVector() * (TileSize * 2 * IndexByX + TileSize);
			FVector TilePosition = GetGridBottomLeft() + OffsetByX + OffsetByY;

			EObstacleType ObstacleType = OT_Normal;

			// Add tile without ground
			if (!SphereTileTrace(TilePosition, PathfindingGlobal::GroundCollisionChannel, ObstacleType))
			{
				AddTileStruct(IndexByX, IndexByY, OT_None, TilePosition);
				continue;
			}

			// Add tile with obstacle 
			if (SphereTileTrace(TilePosition, PathfindingGlobal::ObstacleCollisionChannel, ObstacleType))
				AddTileStruct(IndexByX, IndexByY, ObstacleType, TilePosition);

				// Add tile without obstacle
			else
				AddTileStruct(IndexByX, IndexByY, OT_Normal, TilePosition);
		}
	}
}

void APFGrid::AddTileStruct(const int32& InIndexByX, const int32& InIndexByY, const EObstacleType& InObstacleType, const FVector& InWorldLocation)
{
	const FVector2D CurrentTileIndex{static_cast<float>(InIndexByX), static_cast<float>(InIndexByY)};

	FTileInfo CurrentTileInfo;
	CurrentTileInfo.Index = CurrentTileIndex;
	CurrentTileInfo.ObstacleType = InObstacleType;
	CurrentTileInfo.WorldLocation = InWorldLocation;
	Tiles.Add(CurrentTileIndex, CurrentTileInfo);
}
