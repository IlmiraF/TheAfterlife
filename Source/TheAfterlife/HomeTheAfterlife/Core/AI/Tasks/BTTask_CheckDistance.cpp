// Fill out your copyright notice in the Description page of Project Settings.

#include "BehaviorTree\BlackboardComponent.h"
#include "TheAfterlife\HomeTheAfterlife\Core\Characters\BaseCharacter.h"
#include "BTTask_CheckDistance.h"

UBTTask_CheckDistance::UBTTask_CheckDistance()
{
	NodeName = "Checking Distance To Target";
}

EBTNodeResult::Type UBTTask_CheckDistance::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();

	if (!IsValid(AIController) || !IsValid(Blackboard))
	{
		return EBTNodeResult::Failed;
	}

	ABaseCharacter* Character = Cast<ABaseCharacter>(AIController->GetPawn());

	if (!IsValid(Character))
	{
		return EBTNodeResult::Failed;
	}

	const UCharacterEquipmentComponent* EquipmentComponent = Character->GetCharacterEquipmentComponent();

	ABaseCharacter* CurrentTarget = Cast<ABaseCharacter>(Blackboard->GetValueAsObject(TargetKey.SelectedKeyName));

	if (!IsValid(CurrentTarget))
	{
		return EBTNodeResult::Failed;
	}

	float DistSq = FVector::DistSquared(CurrentTarget->GetActorLocation(), Character->GetActorLocation());


	if (DistSq > FMath::Square(Distance))
	{
		return EBTNodeResult::Failed;
	}

	return EBTNodeResult::Succeeded;
}
