#include "BTTask_MoveToSpline.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "..\..\Characters\Boss\Boss.h"

UBTTask_MoveToSpline::UBTTask_MoveToSpline()
{
	NodeName = "MoveToCircleSplineTask";
}

EBTNodeResult::Type UBTTask_MoveToSpline::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

	if (Blackboard->GetValueAsBool(BB_OnCircleSpline))
	{
		return EBTNodeResult::Failed;
	}

	Boss->SwitchSplines(EBirdFlinghtTypes::Rise);
	Boss->SetSplineMovement(true);

	return EBTNodeResult::Succeeded;
}