#include "BTTask_MakeInvulnerable.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "../../Characters/Boss/Boss.h"

UBTTask_MakeInvulnerable::UBTTask_MakeInvulnerable()
{
	NodeName = "MakeInvulnerableTask";
}

EBTNodeResult::Type UBTTask_MakeInvulnerable::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

	//if (!Blackboard->GetValueAsBool(BB_OnCircleSpline))
	//{
	//	return EBTNodeResult::Failed;
	//}
	//
	//Boss->SwitchSplines(EBirdFlinghtTypes::FlyingInCircle);
	//Boss->SetSplineMovement(true);

	Boss->SetInvulnerable(true);

	return EBTNodeResult::Succeeded;
}