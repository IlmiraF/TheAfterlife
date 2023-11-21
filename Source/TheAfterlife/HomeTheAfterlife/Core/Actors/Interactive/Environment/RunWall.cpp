// Fill out your copyright notice in the Description page of Project Settings.


#include "RunWall.h"
#include "../../../../../TheAfterlifeTypes.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"

ARunWall::ARunWall()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("WallRoot"));

	WallMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Wall Mesh"));
	WallMeshComponent->SetupAttachment(RootComponent);

	LeftInteractionVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("Left Interaction Volume"));
	LeftInteractionVolume->SetupAttachment(RootComponent);
	LeftInteractionVolume->SetCollisionProfileName(CollisionProfilePawnInteractionVolume);
	LeftInteractionVolume->SetGenerateOverlapEvents(true);

	RightInteractionVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("Right Interaction Volume"));
	RightInteractionVolume->SetupAttachment(RootComponent);
	RightInteractionVolume->SetCollisionProfileName(CollisionProfilePawnInteractionVolume);
	RightInteractionVolume->SetGenerateOverlapEvents(true);
}

void ARunWall::OnConstruction(const FTransform& Transform)
{
	UStaticMesh* WallMesh = WallMeshComponent->GetStaticMesh();
	if (IsValid(WallMesh))
	{
		float MeshHeight = WallMesh->GetBoundingBox().GetSize().Z;
		float MeshWidth = WallMesh->GetBoundingBox().GetSize().Y;
		if (!FMath::IsNearlyZero(MeshHeight))
		{
			WallMeshComponent->SetRelativeScale3D(FVector(1.0f, WallWidth / MeshWidth, WallHeight / MeshHeight));
		}
	}

	FVector LeftBoxExtent = LeftInteractionVolume->GetUnscaledBoxExtent();
	LeftInteractionVolume->SetBoxExtent(FVector(LeftBoxExtent.X, LeftBoxExtent.Y, WallHeight));
	LeftInteractionVolume->SetRelativeLocation(FVector(0.0f, 0.0f, WallHeight * 0.5f));

	FVector RightBoxExtent = RightInteractionVolume->GetUnscaledBoxExtent();
	RightInteractionVolume->SetBoxExtent(FVector(RightBoxExtent.X, RightBoxExtent.Y, WallHeight));
	RightInteractionVolume->SetRelativeLocation(FVector(0.0f, WallWidth, WallHeight * 0.5f));
}

void ARunWall::BeginPlay()
{
	Super::BeginPlay();
}
