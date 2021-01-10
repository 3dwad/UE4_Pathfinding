﻿#include "PFTileActor.h"
#include "PFGrid.h"
#include "PFObstacleMaster.h"

namespace PFTileActorLocal
{
	const FName TileColor = "TileColor";
	const FName ColorActivation = "ColorActivation";
}

APFTileActor::APFTileActor()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	RootComponent = SceneRoot;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(RootComponent);
}

const FTileInfo& APFTileActor::GetTileData() const
{
	return GridActor->Tiles.FindChecked(Index);
}

void APFTileActor::SetTileColor()
{
	FColor TileColor = FColor::Green;
	bool bIsColorActivation = true;

	switch (GetTileData().ObstacleType)
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
	case OT_Normal:
		bIsColorActivation = false;
	default: ;
	}

	StaticMeshComponent->SetVectorParameterValueOnMaterials(PFTileActorLocal::TileColor, FVector{TileColor});
	StaticMeshComponent->SetScalarParameterValueOnMaterials(PFTileActorLocal::ColorActivation, bIsColorActivation);
}

void APFTileActor::OnTileHovered(UPrimitiveComponent* InTouchedComponent)
{
	bHovered = true;
}

void APFTileActor::OnTileUnhovered(UPrimitiveComponent* InTouchedComponent)
{
	bHovered = false;
}

void APFTileActor::BeginPlay()
{
	Super::BeginPlay();

	StaticMeshComponent->OnBeginCursorOver.AddDynamic(this, &APFTileActor::OnTileHovered);
	StaticMeshComponent->OnEndCursorOver.AddDynamic(this, &APFTileActor::OnTileUnhovered);
	SetTileColor();
}
