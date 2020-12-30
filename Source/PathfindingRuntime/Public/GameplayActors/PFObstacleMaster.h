#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PFObstacleMaster.generated.h"

UENUM()
enum EObstacleType
{
	OT_Normal UMETA(DisplayName="Normal"),
	OT_Complicated UMETA(DisplayName="Complicated"),
	OT_Difficult UMETA(DisplayName="Difficult"),
	OT_Impassable UMETA(DisplayName="Impassable"),
	MAX
};

UCLASS()
class PATHFINDINGRUNTIME_API APFObstacleMaster : public AActor
{
	GENERATED_BODY()

public:
	APFObstacleMaster();

	UPROPERTY()
	USceneComponent* SceneRoot;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(EditAnywhere, Category="Settings")
	TEnumAsByte<EObstacleType> ObstacleType{OT_Complicated};

protected:
	virtual void BeginPlay() override;
};
