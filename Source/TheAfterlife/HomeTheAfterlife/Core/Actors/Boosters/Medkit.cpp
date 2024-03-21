// Fill out your copyright notice in the Description page of Project Settings.


#include "Medkit.h"
#include "../../Characters/BaseCharacter.h"
#include "../../Components/CharacterComponents/CharacterAttributeComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "../../../../TheAfterlifeTypes.h"

AMedkit::AMedkit()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("VolumeRoot"));

	InteractionVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("Interaction Volume"));
	InteractionVolume->SetupAttachment(RootComponent);
	InteractionVolume->SetCollisionProfileName(CollisionProfilePawnInteractionVolume);
	InteractionVolume->SetGenerateOverlapEvents(true);

	MedkitMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Medkit Mesh"));
	MedkitMeshComponent->SetupAttachment(RootComponent);
}

void AMedkit::BeginPlay()
{
	Super::BeginPlay();
	if (IsValid(InteractionVolume))
	{
		InteractionVolume->OnComponentBeginOverlap.AddDynamic(this, &AMedkit::OnInteractionVolumeOverlapBegin);
	}
}

void AMedkit::Consume(ABaseCharacter* ConsumeTarget)
{
	UCharacterAttributeComponent* CharacterAttributes = ConsumeTarget->GetCharacterAttributeComponent();
	CharacterAttributes->AddHealth(Health);
	Destroy();
}

void AMedkit::OnInteractionVolumeOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(OtherActor);
	if (!IsValid(BaseCharacter))
	{
		return;
	}

	if (Cast<UCapsuleComponent>(OtherComp) != BaseCharacter->GetCapsuleComponent())
	{
		return;
	}

	Consume(BaseCharacter);
}