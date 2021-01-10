#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "PFObstacleMaster.h"
#include "PFGrid.generated.h"

class APFTileActor;

USTRUCT()
struct FTileInfo
{
	GENERATED_BODY()

	FVector2D Index{0.f};
	FVector WorldLocation{0.f};
	EObstacleType ObstacleType{OT_Normal};
};

UCLASS()
class PATHFINDINGRUNTIME_API APFGrid : public AActor
{
	GENERATED_BODY()

public:
	APFGrid();

	UPROPERTY()
	USceneComponent* SceneRoot;

	UPROPERTY()
	UBillboardComponent* Billboard;

	//	AActor interfaces start
	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;
	//	AActor interfaces end

	UFUNCTION(BlueprintCallable, Category="Construction")
	void ConstructionScriptLogic();

	FVector GetGridBottomLeft() const;
	void GetGridTileNumber(int32& OutGridTileNumberX, int32& OutGridTileNumberY) const;
	void SpawnTileActors();
	void DrawTile();

	// Find ground under tile
	bool SphereTileTrace(const FVector& TileLocation, const ECollisionChannel& InCollisionChannel, EObstacleType& OutObstacleType);

	EObstacleType GetObstacleType(const TWeakObjectPtr<AActor>& InActor) const;
	void GenerateMapDataFromWorld();
	void AddTileStruct(const int32& InIndexByX, const int32& InIndexByY, const EObstacleType& InObstacleType, const FVector& InWorldLocation);
public:

	UPROPERTY(EditInstanceOnly, Category="Settings")
	FVector GridWorldLocation;

	UPROPERTY(EditInstanceOnly, Category="Settings")
	FVector2D GridSizeWorld{500.f, 500.f};

	UPROPERTY(EditInstanceOnly, Category="Settings")
	float TileSize{50.f};

	UPROPERTY(EditInstanceOnly, Category="Settings")
	float TileSizeMinus{5.f};

	UPROPERTY(EditInstanceOnly, Category="Settings")
	TSubclassOf<APFTileActor> TileActorClass;

	UPROPERTY(EditInstanceOnly, Category="Debug")
	FColor GridBoxColor{FColor::Blue};

	UPROPERTY(VisibleAnywhere, Category="Grid")
	TMap<FVector2D, FTileInfo> Tiles;
};
