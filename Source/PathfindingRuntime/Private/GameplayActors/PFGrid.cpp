#include "PFGrid.h"


#include "DrawDebugHelpers.h"
#include "Components/BillboardComponent.h"

APFGrid::APFGrid()
{
	//PrimaryActorTick.bCanEverTick = true;

	SceneRoot = CreateDefaultSubobject<USceneComponent>("SceneRoot");
	RootComponent = SceneRoot;

	Billboard = CreateDefaultSubobject<UBillboardComponent>("Billboard");
	Billboard->SetupAttachment(GetRootComponent());
}

void APFGrid::OnConstruction(const FTransform& Transform)
{

	FlushPersistentDebugLines(GetWorld());
	GridLocation = SceneRoot->GetComponentLocation();
	DrawDebugBox(GetWorld(), GridLocation, FVector(GridSizeWorld.Y, GridSizeWorld.X, 5.f), GridBoxColor,false,600.f,SDPG_World,10);
	DrawDebugSphere(GetWorld(), GetGridBottomLeft(), 30.f, 5, FColor::Red, false, 600.f, SDPG_World, 4);
	DrawTile();

	Super::OnConstruction(Transform);
}

void APFGrid::BeginPlay()
{
	Super::BeginPlay();
}

FVector APFGrid::GetGridBottomLeft() const
{
	return GridLocation - (SceneRoot->GetRightVector() * GridSizeWorld.X) - (SceneRoot->GetForwardVector() * GridSizeWorld.Y);
}

void APFGrid::GetGridTileNumber(int32& OutGridTileNumberX, int32& OutGridTileNumberY) const
{
	OutGridTileNumberX = static_cast<int32>(FMath::RoundToZero((GridSizeWorld / TileSize).X));
	OutGridTileNumberY = static_cast<int32>(FMath::RoundToZero((GridSizeWorld / TileSize).Y));
}

void APFGrid::DrawTile()
{
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

			FPlane SomePlane = FPlane(0.f, 0.f, 1.f, GridLocation.Z);
			DrawDebugSolidPlane(GetWorld(), SomePlane, TilePosition, TileSize - 5, FColor::Orange, false, 600.f);
		}
	}
}

void APFGrid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

