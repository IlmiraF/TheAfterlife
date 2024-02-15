// Fill out your copyright notice in the Description page of Project Settings.


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
