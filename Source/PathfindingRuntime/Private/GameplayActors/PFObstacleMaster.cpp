#include "GameplayActors/PFObstacleMaster.h"

APFObstacleMaster::APFObstacleMaster()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	RootComponent = SceneRoot;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetCollisionResponseToChannel(ECC_GameTraceChannel2,ECR_Block);
	StaticMeshComponent->SetupAttachment(RootComponent);
}

void APFObstacleMaster::BeginPlay()
{
	Super::BeginPlay();
}
