#include "BTTask_Concussion.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "../../Characters/Boss/Boss.h"

UBTTask_Concussion::UBTTask_Concussion()
{
	NodeName = "ConcussionTask";
}

EBTNodeResult::Type UBTTask_Concussion::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

	if (!Blackboard->GetValueAsBool(BB_BossConcussed))
	{
		return EBTNodeResult::Failed;
	}

	if (!Blackboard->GetValueAsBool(BB_OnGround))
	{
		return EBTNodeResult::Failed;
	}

	Boss->Concussion();

	return EBTNodeResult::Succeeded;
}
