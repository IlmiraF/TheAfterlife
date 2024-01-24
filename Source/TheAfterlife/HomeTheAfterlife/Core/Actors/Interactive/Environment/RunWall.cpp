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

	InteractionVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("Interaction Volume"));
	InteractionVolume->SetupAttachment(RootComponent);
	InteractionVolume->SetCollisionProfileName(CollisionProfilePawnInteractionVolume);
	InteractionVolume->SetGenerateOverlapEvents(true);
}

void ARunWall::OnConstruction(const FTransform& Transform)
{
	WallMeshComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));

	UStaticMesh* WallMesh = WallMeshComponent->GetStaticMesh();
	if (IsValid(WallMesh))
	{
		float MeshHeight = WallMesh->GetBoundingBox().GetSize().Z;
		float MeshWidth = WallMesh->GetBoundingBox().GetSize().Y;
		if (!FMath::IsNearlyZero(MeshHeight))
		{
			WallMeshComponent->SetRelativeScale3D(FVector(1.0f, WallWidth/ MeshWidth, WallHeight / MeshHeight));
		}
	}

	float BoxDepthExtent = GetInteractionBox()->GetUnscaledBoxExtent().X;
	GetInteractionBox()->SetBoxExtent(FVector(BoxDepthExtent, WallWidth * 0.5f, WallHeight * 0.5f));
	GetInteractionBox()->SetRelativeLocation(FVector(WallMesh->GetBoundingBox().GetSize().X + BoxDepthExtent, WallWidth * 0.5f, WallHeight * 0.5f));
}

UBoxComponent* ARunWall::GetInteractionBox() const
{
	return StaticCast<UBoxComponent*>(InteractionVolume);
}
