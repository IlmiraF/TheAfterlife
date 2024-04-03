// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_CheckPlayerIsAlive.h"
#include "AIController.h"
#include "BehaviorTree\BlackboardComponent.h"
#include "../../Characters\BaseCharacter.h"
#include "../../Components\CharacterComponents\CharacterAttributeComponent.h"

UBTTask_CheckPlayerIsAlive::UBTTask_CheckPlayerIsAlive()
{
	NodeName = "Checking If Player Is Alive";
}

EBTNodeResult::Type UBTTask_CheckPlayerIsAlive::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

	AAIController* AIController = OwnerComp.GetAIOwner();
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();

	if (!IsValid(AIController) || !IsValid(Blackboard))
	{
		return EBTNodeResult::Failed;
	}

	ABaseCharacter* CurrentTarget = Cast<ABaseCharacter>(Blackboard->GetValueAsObject(TargetKey.SelectedKeyName));

	if (!IsValid(CurrentTarget))
	{
		return EBTNodeResult::Failed;
	}

	UCharacterAttributeComponent* AttributeComponent = CurrentTarget->GetCharacterAttributeComponent();

	if (!AttributeComponent->IsAlive())
	{	
		//Blackboard->ClearValue(TargetKey.SelectedKeyName);
		return EBTNodeResult::Failed;
	}

	return EBTNodeResult::Succeeded;
}
