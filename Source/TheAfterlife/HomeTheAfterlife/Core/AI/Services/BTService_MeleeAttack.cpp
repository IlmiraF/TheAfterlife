#include "BTService_MeleeAttack.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "../../Characters/BaseCharacter.h"
#include "TheAfterlife\HomeTheAfterlife\Core\Components\CharacterComponents\CharacterEquipmentComponent.h"
#include "TheAfterlife/HomeTheAfterlife/Core/Actors/Equipment/Weapons/MeleeWeaponItem.h"

UBTService_MeleeAttack::UBTService_MeleeAttack()
{
	NodeName = "Melee Attack";
}

void UBTService_MeleeAttack::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AAIController* AIController = OwnerComp.GetAIOwner();
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();

	if (!IsValid(AIController) || !IsValid(Blackboard))
	{
		return;
	}

	ABaseCharacter* Character = Cast<ABaseCharacter>(AIController->GetPawn());

	if (!IsValid(Character))
	{
		return;
	}

	const UCharacterEquipmentComponent* EquipmentComponent = Character->GetCharacterEquipmentComponent();

	AMeleeWeaponItem* MeleeWeapon = EquipmentComponent->GetCurrentMeleeWeaponItem();

	if (!IsValid(MeleeWeapon))
	{
		return;
	}

	AActor* CurrentTarget = Cast<AActor>(Blackboard->GetValueAsObject(TargetKey.SelectedKeyName));


	if (!IsValid(CurrentTarget))
	{
		return;
	}

	if (MeleeWeapon->IsAttacking())
	{
		return;
	}

	Character->HandsMeleeAttack();
}
