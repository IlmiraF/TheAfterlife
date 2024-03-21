// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacterAnimInstance.h"
#include "../BaseCharacter.h"
#include "../../../../TheAfterlifeTypes.h"
#include "../../Components/CharacterComponents/CharacterEquipmentComponent.h"
#include "../../Components/MovementComponents/BaseCharacterMovementComponent.h"

void UBaseCharacterAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
	checkf(TryGetPawnOwner()->IsA<ABaseCharacter>(), TEXT("UBaseCharacterAnimInstance::NativeBeginPlay() UBaseCharacterAnimInstance can be used only with ABaseCharacter"));
	CachedBaseCharacter = StaticCast<ABaseCharacter*>(TryGetPawnOwner());
}

void UBaseCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!CachedBaseCharacter.IsValid())
	{
		return;
	}

	UBaseCharacterMovementComponent* CharacterMovement = CachedBaseCharacter->GetBaseCharacterMovementComponent();
	Speed = CharacterMovement->Velocity.Size();
	bIsFalling = CharacterMovement->IsFalling();
	bIsCrouching = CharacterMovement->IsCrouching();
	bIsOnLadder = CharacterMovement->IsOnLadder();
	bIsClimbing = CharacterMovement->IsClimbing();
	bIsOnBeam = CharacterMovement->IsOnBeam();

	if (bIsOnLadder)
	{
		LadderSpeedRatio = CharacterMovement->GetLadderSpeedRatio();
	}

	if (bIsOnBeam)
	{
		OnBeamDirection = CharacterMovement->GetOnBeamDirection();
	}
	ClimbVelocity = CharacterMovement->GetUnrotatedClimbVelocity();

	const UCharacterEquipmentComponent* CharacterEquipment = CachedBaseCharacter->GetCharacterEquipmentComponent();
	
	CurrentEquippedItemType = CharacterEquipment->GetCurrentEquippedItemType();
}
