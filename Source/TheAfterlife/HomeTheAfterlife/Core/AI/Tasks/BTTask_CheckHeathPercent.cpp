#include "BTTask_CheckHeathPercent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "../../Components/CharacterComponents/CharacterAttributeComponent.h"
#include "../../Characters/Boss/Boss.h"

UBTTask_CheckHeathPercent::UBTTask_CheckHeathPercent()
{
	NodeName = "CheckHealthPercentTask";
}

EBTNodeResult::Type UBTTask_CheckHeathPercent::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

	UCharacterAttributeComponent* AttributeComponent = Boss->GetCharacterAttributeComponent();

	if (!IsValid(AttributeComponent))
	{
		return EBTNodeResult::Failed;
	}

	float HealthPercent = AttributeComponent->GetHealthPercent();

	UE_LOG(LogDamage, Warning, TEXT("HealthPercent: %f"), HealthPercent);

	if (HealthPercent > MaxPercent)
	{
		return EBTNodeResult::Failed;
	}

	if (HealthPercent < MinPercent)
	{
		return EBTNodeResult::Failed;
	}

	return EBTNodeResult::Succeeded;
}
