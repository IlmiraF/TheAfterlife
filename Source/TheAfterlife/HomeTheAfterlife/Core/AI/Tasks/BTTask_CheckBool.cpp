#include "BTTask_CheckBool.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "..\..\Characters\Boss\Boss.h"

UBTTask_CheckBool::UBTTask_CheckBool()
{
	NodeName = "Check Bool";
}

EBTNodeResult::Type UBTTask_CheckBool::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();

	if (!IsValid(AIController) || !IsValid(Blackboard))
	{
		return EBTNodeResult::Failed;
	}

	bool Bool = Blackboard->GetValueAsBool(TargetKey.SelectedKeyName);

	//UE_LOG(LogDamage, Warning, TEXT("Bool value: %d"), Bool);

	if (Bool != TargetValue)
	{
		return EBTNodeResult::Failed;
	}

	return EBTNodeResult::Succeeded;
}