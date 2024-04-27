#include "BTTask_RestoreHealth.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "../../Components/CharacterComponents/CharacterAttributeComponent.h"
#include "../../Characters/Boss/Boss.h"

UBTTask_RestoreHealth::UBTTask_RestoreHealth()
{
	NodeName = "RestoreHealth";
}

EBTNodeResult::Type UBTTask_RestoreHealth::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

	if (Boss->GetSecondStage())
	{
		return EBTNodeResult::Failed;
	}

	Boss->StartSecondStage();

	return EBTNodeResult::Succeeded;
}

