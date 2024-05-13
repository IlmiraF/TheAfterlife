// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseAICharacter.h"
#include "../../Components/CharacterComponents/CharacterAttributeComponent.h"
#include "../../Components/MovementComponents/BaseCharacterMovementComponent.h"
#include "../../Components/CharacterComponents/CharacterEquipmentComponent.h"
#include "../../Actors/Equipment/Weapons/RangeWeaponItem.h"
#include "Components/CapsuleComponent.h"


ABaseAICharacter::ABaseAICharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	AIPatrollingComponent = CreateDefaultSubobject<UAIPatrollingComponent>(TEXT("AIPatrolling"));
}

UAIPatrollingComponent* ABaseAICharacter::GetPatrollingComponent() const
{
	return AIPatrollingComponent;
}

UBehaviorTree* ABaseAICharacter::GetBehaviorTree() const
{
	return BehaviorTree;
}


void ABaseAICharacter::OnDeath()
{
	Super::OnDeath();

	if (!CanRevival)
	{	
		USkeletalMeshComponent* MeshComp = GetMesh();
		MeshComp->SetVisibility(false);
		MeshComp->SetCollisionProfileName(CollisionProfileRagdoll);
		MeshComp->SetSimulatePhysics(true);
		SetActorEnableCollision(false);
		SetActorTickEnabled(false);

		ARangeWeaponItem* RangeWeapon = GetCharacterEquipmentComponent_Mutable()->GetCurrentRangeWeapon();
		if (IsValid(RangeWeapon))
		{
			RangeWeapon->StopFire();
		}
		
		Destroy();
	}
	else
	{
		GetCharacterMovement()->DisableMovement();

		PlayAnimMontage(OnDeathAnimMontage);
	}

	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("OnDeath")));
}

void ABaseAICharacter::DisableCharacter()
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("DisableCharacter")));
	if (IsValid(CharacterEquipmentComponent))
	{
		CharacterEquipmentComponent->HideWeapons();
	}

	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("DisableCharacter")));
	if (OnCharacterDeath.IsBound())
	{
		OnCharacterDeath.Broadcast(this);
	}
	else
	{
		USkeletalMeshComponent* MeshComp = GetMesh();
		MeshComp->SetVisibility(false);
		MeshComp->SetCollisionProfileName(CollisionProfileRagdoll);
		MeshComp->SetSimulatePhysics(true);
		SetActorEnableCollision(false);
		SetActorTickEnabled(false);
	}

}

void ABaseAICharacter::Revival()
{
	GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Red, FString::Printf(TEXT("HLOP")));

	CharacterAttributesComponent->RestoreHealth();

	GetCharacterMovement()->SetMovementMode(MOVE_NavWalking);

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}