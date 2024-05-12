// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelTransition.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "../../../Characters/BaseCharacter.h"
#include "../../../../../TheAfterlifeTypes.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ALevelTransition::ALevelTransition()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("VolumeRoot"));

	InteractionVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	InteractionVolume->SetupAttachment(RootComponent);
	InteractionVolume->SetCollisionProfileName(CollisionProfilePawnInteractionVolume);
	InteractionVolume->SetGenerateOverlapEvents(true);
}

void ALevelTransition::BeginPlay()
{
	Super::BeginPlay();
	if (IsValid(InteractionVolume))
	{
		InteractionVolume->OnComponentBeginOverlap.AddDynamic(this, &ALevelTransition::OnInteractionVolumeOverlapBegin);
	}
}

void ALevelTransition::OnInteractionVolumeOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!IsOverlappingCharacterCapsule(OtherActor, OtherComp))
	{
		return;
	}

	UGameplayStatics::OpenLevel(GetWorld(), LevelName);
}

bool ALevelTransition::IsOverlappingCharacterCapsule(AActor* OtherActor, UPrimitiveComponent* OtherComp)
{
	ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(OtherActor);
	if (!IsValid(BaseCharacter))
	{
		return false;
	}

	if (Cast<UCapsuleComponent>(OtherComp) != BaseCharacter->GetCapsuleComponent())
	{
		return false;
	}
	return true;
}