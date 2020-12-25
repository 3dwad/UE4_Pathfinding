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
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;
	//	AActor interfaces end

	FVector GetGridBottomLeft() const;
	void GetGridTileNumber(int32& OutGridTileNumberX, int32& OutGridTileNumberY) const;
	void DrawTile();
public:

	UPROPERTY(VisibleAnywhere, Category="Settings")
	FVector GridLocation;

	UPROPERTY(VisibleAnywhere, Category="Settings")
	FVector2D GridSizeWorld{500.f, 500.f};

	UPROPERTY(VisibleAnywhere, Category="Settings")
	float TileSize{50.f};

	UPROPERTY(VisibleAnywhere, Category="Debug")
	FColor GridBoxColor{FColor::Blue};
};
