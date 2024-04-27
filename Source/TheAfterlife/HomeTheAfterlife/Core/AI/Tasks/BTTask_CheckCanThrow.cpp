#include "BTTask_CheckCanThrow.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "../../Characters/BaseCharacter.h"
#include "../../Components/CharacterComponents/CharacterEquipmentComponent.h"
#include "../../Actors/Equipment/Throwables/ThrowableItem.h"

UBTTask_CheckCanThrow::UBTTask_CheckCanThrow()
{
	NodeName = "CheckCanThrow";
}

EBTNodeResult::Type UBTTask_CheckCanThrow::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();

	if (!IsValid(AIController) || !IsValid(Blackboard))
	{
		return EBTNodeResult::Failed;
	}

	ABaseCharacter* Character = Cast<ABaseCharacter>(AIController->GetPawn());

	if (!IsValid(Character))
	{
		return EBTNodeResult::Failed;
	}

	UCharacterEquipmentComponent* EquipmentComponent = Character->GetCharacterEquipmentComponent_Mutable();

	if (!IsValid(EquipmentComponent))
	{
		return EBTNodeResult::Failed;
	}
	
	AThrowableItem* CurrentThrowableItem = EquipmentComponent->GetCurrentThrowableItem();

	if (!IsValid(CurrentThrowableItem))
	{	
		return EBTNodeResult::Failed;
	}

	if (!CurrentThrowableItem->CanThrow())
	{
		return EBTNodeResult::Failed;
	}

	return EBTNodeResult::Succeeded;
}
