// Fill out your copyright notice in the Description page of Project Settings.

#include "..\..\Components\CharacterComponents\CharacterAttributeComponent.h"
#include "..\..\Components\MovementComponents\BaseCharacterMovementComponent.h"
#include "BaseAICharacter.h"
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

	GetCharacterMovement()->DisableMovement();

	PlayAnimMontage(OnDeathAnimMontage);
}

void ABaseAICharacter::DisableCharacter()
{	
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

	//GetMesh()->SetVisibility(false);
	//SetActorEnableCollision(false);
	//GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//SetActorTickEnabled(false);
	//SetActorHiddenInGame(true);

}

void ABaseAICharacter::Revival()
{
	CharacterAttributesComponent->RestoreHealth();
	//SetActorHiddenInGame(false);

	GetCharacterMovement()->SetMovementMode(MOVE_NavWalking);
	//GetMesh()->SetVisibility(true);
	//SetActorEnableCollision(true);
	//SetActorTickEnabled(true);

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}