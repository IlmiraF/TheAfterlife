#include "BTTask_CheckIsAltarsIntact.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "../../Characters/Boss/Boss.h"
#include "../../AI/Controllers/BossAIController.h"
#include "TheAfterlife/TheAfterlifeTypes.h"

UBTTask_CheckIsAltarsIntact::UBTTask_CheckIsAltarsIntact()
{
	NodeName = "Check Is Altars Intact";
}

EBTNodeResult::Type UBTTask_CheckIsAltarsIntact::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();

	if (!IsValid(AIController) || !IsValid(Blackboard))
	{
		return EBTNodeResult::Failed;
	}

	if (Blackboard->GetValueAsBool(BB_AltarsDestroyed))
	{
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}