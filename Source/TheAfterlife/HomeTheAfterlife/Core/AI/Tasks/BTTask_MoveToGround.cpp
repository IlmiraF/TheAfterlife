#include "BTTask_MoveToGround.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "../../Characters/Boss/Boss.h"

UBTTask_MoveToGround::UBTTask_MoveToGround()
{
	NodeName = "MoveToGroundTask";
}

EBTNodeResult::Type UBTTask_MoveToGround::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

	if (Blackboard->GetValueAsBool(BB_OnGround))
	{
		return EBTNodeResult::Failed;
	}

	Boss->SetMovementToGround(true);

	return EBTNodeResult::Succeeded;
}