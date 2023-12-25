// Fill out your copyright notice in the Description page of Project Settings.


#include "Beam.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Components/BoxComponent.h"
#include "../../../../../TheAfterlifeTypes.h"
#include "../../../Characters/BaseCharacter.h"

ABeam::ABeam()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("BeamRoot"));

	BeamMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Beam Mesh"));
	BeamMeshComponent->SetupAttachment(RootComponent);

	InteractionVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("Interaction Volume"));
	InteractionVolume->SetupAttachment(RootComponent);
	InteractionVolume->SetCollisionProfileName(CollisionProfilePawnInteractionVolume);
	InteractionVolume->SetGenerateOverlapEvents(true);
}

void ABeam::OnConstruction(const FTransform& Transform)
{
	UStaticMesh* StepsMesh = BeamMeshComponent->GetStaticMesh();
	if (IsValid(StepsMesh))
	{
		float MeshWidth = StepsMesh->GetBoundingBox().GetSize().Y;
		float MeshHeight = StepsMesh->GetBoundingBox().GetSize().Z;
		if (!FMath::IsNearlyZero(MeshWidth))
		{
			BeamMeshComponent->SetRelativeScale3D(FVector(0.5f, BeamWidth / MeshWidth, BeamHeight / MeshHeight));
		}
	}

	float BoxDepthExtent = GetBeamInteractionBox()->GetUnscaledBoxExtent().X;
	GetBeamInteractionBox()->SetBoxExtent(FVector(50.0f, BeamWidth * 0.5f, BeamHeight * 0.5f));
	GetBeamInteractionBox()->SetRelativeLocation(FVector(0.0f, 0.0f, BeamHeight));
}

float ABeam::GetBeamWidth() const
{
	return BeamWidth;
}

UBoxComponent* ABeam::GetBeamInteractionBox() const
{
	return StaticCast<UBoxComponent*>(InteractionVolume);
}

void ABeam::OnInteractionVolumeOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnInteractionVolumeOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	ABaseCharacter* BaseCharacter = StaticCast<ABaseCharacter*>(OtherActor);
	BaseCharacter->InteractWithBeam();
}

void ABeam::OnInteractionVolumeOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnInteractionVolumeOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);

	ABaseCharacter* BaseCharacter = StaticCast<ABaseCharacter*>(OtherActor);
	BaseCharacter->InteractWithBeam();
}