// Fill out your copyright notice in the Description page of Project Settings.

#include "..\..\Components\CharacterComponents\CharacterAttributeComponent.h"
#include "..\..\Components\MovementComponents\BaseCharacterMovementComponent.h"
#include "BaseAICharacter.h"


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

}

void ABaseAICharacter::Revival()
{
	CharacterAttributesComponent->RestoreHealth();

	USkeletalMeshComponent* MeshComp = this->GetMesh();
	MeshComp->SetVisibility(true);
	MeshComp->SetSimulatePhysics(false);
	this->SetActorEnableCollision(true);
	this->SetActorTickEnabled(true);
	

	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, TEXT("GOVNO"));
}
