#include "BTTask_SplineMovement.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "..\..\Characters\Boss\Boss.h"


UBTTask_SplineMovement::UBTTask_SplineMovement()
{
	NodeName = "SplineMovementTask";
}

EBTNodeResult::Type UBTTask_SplineMovement::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();

	if (!IsValid(AIController) || !IsValid(Blackboard))
	{
		return EBTNodeResult::Failed;
	}

	ABoss* Boss = Cast<ABoss>(AIController->GetPawn());

	if (!IsValid(Boss))
	{
		return EBTNodeResult::Failed;
	}

	if (Blackboard->GetValueAsBool(BB_OnSpline))
	{
		return EBTNodeResult::Failed;
	}

	Boss->CanFlyToSpline = true;

	return EBTNodeResult::Succeeded;
}