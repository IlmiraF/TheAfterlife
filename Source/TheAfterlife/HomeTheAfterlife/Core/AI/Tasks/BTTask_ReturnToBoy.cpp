#include "BTTask_ReturnToBoy.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "../../Characters/Boss/Boss.h"

UBTTask_ReturnToBoy::UBTTask_ReturnToBoy()
{
	NodeName = "ReturnToBoy";
}

EBTNodeResult::Type UBTTask_ReturnToBoy::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

	if (Blackboard->GetValueAsBool(BB_IsBoy))
	{
		return EBTNodeResult::Failed;
	}

	Boss->ReturnToBoy();

	return EBTNodeResult::Succeeded;
}
