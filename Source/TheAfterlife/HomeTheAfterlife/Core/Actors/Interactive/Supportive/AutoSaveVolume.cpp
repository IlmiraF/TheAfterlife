// Fill out your copyright notice in the Description page of Project Settings.


#include "AutoSaveVolume.h"
#include "../../../Characters/BaseCharacter.h"
#include "../../../Components/CharacterComponents/CharacterAttributeComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "../../../Subsystems/SaveSubsystem/SaveSubsystem.h"
#include "../../../../../TheAfterlifeTypes.h"

AAutoSaveVolume::AAutoSaveVolume()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("VolumeRoot"));

	InteractionVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("Interaction Volume"));
	InteractionVolume->SetupAttachment(RootComponent);
	InteractionVolume->SetCollisionProfileName(CollisionProfilePawnInteractionVolume);
	InteractionVolume->SetGenerateOverlapEvents(true);
}

void AAutoSaveVolume::BeginPlay()
{
	Super::BeginPlay();
	if (IsValid(InteractionVolume))
	{
		InteractionVolume->OnComponentBeginOverlap.AddDynamic(this, &AAutoSaveVolume::OnInteractionVolumeOverlapBegin);
	}
}

void AAutoSaveVolume::OnInteractionVolumeOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!IsOverlappingCharacterCapsule(OtherActor, OtherComp))
	{
		return;
	}

	if (bIsSaved)
	{
		return;
	}

	bIsSaved = true;

	USaveSubsystem* SaveSubsystem = UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem<USaveSubsystem>();
	SaveSubsystem->SaveGame();

	if (bIsStealingHealth)
	{
		ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(OtherActor);
		if (IsValid(BaseCharacter))
		{
			BaseCharacter->GetCharacterAttributeComponent()->StealHealth(HealthStealingRatio);
		}
	}
}

bool AAutoSaveVolume::IsOverlappingCharacterCapsule(AActor* OtherActor, UPrimitiveComponent* OtherComp)
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