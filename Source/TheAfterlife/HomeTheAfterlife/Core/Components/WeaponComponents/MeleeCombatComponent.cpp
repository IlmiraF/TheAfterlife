// Fill out your copyright notice in the Description page of Project Settings.


#include "MeleeCombatComponent.h"
#include <GameFramework/Character.h>
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"

// Sets default values for this component's properties
UMeleeCombatComponent::UMeleeCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	//BaseCharacter = Cast<ABaseCharacter>(GetOwner());

	//LeftHandCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("LeftHandCollisionBox"));
	//RightHandCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("RightHandCollisionBox"));
	//
	//if (BaseCharacter)
	//{
	//	LeftHandCollision->SetupAttachment(BaseCharacter->GetRootComponent());
	//	LeftHandCollision->SetHiddenInGame(false);
	//	LeftHandCollision->SetCollisionProfileName("NoCollision");
	//
	//	RightHandCollision->SetupAttachment(BaseCharacter->GetRootComponent());
	//	RightHandCollision->SetHiddenInGame(false);
	//	RightHandCollision->SetCollisionProfileName("NoCollision");
	//}
	
}


// Called when the game starts
void UMeleeCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	const FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, false);

	//if (BaseCharacter)
	//{
	//	LeftHandCollision->AttachToComponent(BaseCharacter->GetMesh(), AttachmentRules, "hand_left_collision");
	//	
	//	RightHandCollision->AttachToComponent(BaseCharacter->GetMesh(), AttachmentRules, "hand_right_collision");
	//}
}



// Called every frame
void UMeleeCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UMeleeCombatComponent::MeleeAttackStart()
{
	GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Green, FString::Printf(TEXT("ATTACK START")));
}

void UMeleeCombatComponent::MeleeAttackFinish()
{

}


