#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"
#include "PFGrid.generated.h"

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
	void DrawTile();

	// Find ground under tile
	bool TraceForGroundDetection(const FVector& TileLocation) const;
public:

	UPROPERTY(EditInstanceOnly, Category="Settings")
	FVector GridLocation;

	UPROPERTY(EditInstanceOnly, Category="Settings")
	FVector2D GridSizeWorld{500.f, 500.f};

	UPROPERTY(EditInstanceOnly, Category="Settings")
	float TileSize{50.f};

	UPROPERTY(EditInstanceOnly, Category="Settings")
	float TileSizeMinus{5.f};

	UPROPERTY(EditInstanceOnly, Category="Debug")
	FColor GridBoxColor{FColor::Blue};
};
