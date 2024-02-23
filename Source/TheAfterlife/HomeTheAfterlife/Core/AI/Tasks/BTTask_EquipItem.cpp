#include "BTTask_EquipItem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "../../Characters/BaseCharacter.h"

UBTTask_EquipItem::UBTTask_EquipItem()
{
	NodeName = "Equip Item";
}

EBTNodeResult::Type UBTTask_EquipItem::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

	EquipmentComponent->EquipItemInSlot(EquipItem);
	return EBTNodeResult::Succeeded;
	
}