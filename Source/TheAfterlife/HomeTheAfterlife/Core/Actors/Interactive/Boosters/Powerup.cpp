// Fill out your copyright notice in the Description page of Project Settings.


#include "Powerup.h"
#include "../Interactive.h"
#include "Components/BoxComponent.h"
#include "../../../../../TheAfterlifeTypes.h"
#include "../../../Characters/BaseCharacter.h"
#include "../../../Components/CharacterComponents/CharacterAttributeComponent.h"

APowerup::APowerup()
{
	USceneComponent* DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(DefaultSceneRoot);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PowerupMesh"));
	Mesh->SetupAttachment(GetRootComponent());

	InteractionVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractionVolume"));
	InteractionVolume->SetupAttachment(GetRootComponent());
	InteractionVolume->SetCollisionProfileName(PickableItemProfile);

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
}

void APowerup::Interact(ABaseCharacter* ConsumeTarget)
{
	UCharacterAttributeComponent* CharacterAttributes = ConsumeTarget->GetCharacterAttributeComponent();
	CharacterAttributes->AddHealth(HealthToAdd);
	this->ConditionalBeginDestroy();
}

bool APowerup::IsForce()
{
	return false;
}
