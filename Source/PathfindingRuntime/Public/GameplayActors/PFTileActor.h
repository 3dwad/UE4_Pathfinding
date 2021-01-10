#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PFTileActor.generated.h"

class APFGrid;
struct FTileInfo;

UCLASS()
class PATHFINDINGRUNTIME_API APFTileActor : public AActor
{
	GENERATED_BODY()

public:
	APFTileActor();

	UPROPERTY()
	USceneComponent* SceneRoot;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(VisibleAnywhere, Category="Tile")
	FVector2D Index;

	UPROPERTY(VisibleAnywhere, Category="Tile")
	APFGrid* GridActor;

	UPROPERTY(VisibleAnywhere, Category="Tile")
	bool bHovered;

	const FTileInfo& GetTileData() const;
	void SetTileColor();

	UFUNCTION()
	void OnTileHovered(UPrimitiveComponent* InTouchedComponent);

	UFUNCTION()
	void OnTileUnhovered(UPrimitiveComponent* InTouchedComponent);

protected:
	virtual void BeginPlay() override;
};
