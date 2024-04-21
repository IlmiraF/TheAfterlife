#include "BTTask_MakeVulnerable.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "../../Characters/Boss/Boss.h"


UBTTask_MakeVulnerable::UBTTask_MakeVulnerable()
{
	NodeName = "MakeVulnerableTask";
}

EBTNodeResult::Type UBTTask_MakeVulnerable::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

	Boss->SetInvulnerable(false);

	return EBTNodeResult::Succeeded;
}
