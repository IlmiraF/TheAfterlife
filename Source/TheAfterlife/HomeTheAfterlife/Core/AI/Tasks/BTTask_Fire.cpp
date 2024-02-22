
#include "BTTask_Fire.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "../../Characters/BaseCharacter.h"
#include "TheAfterlife/HomeTheAfterlife/Core/Actors/Equipment/Weapons/RangeWeaponItem.h"
#include "TheAfterlife\HomeTheAfterlife\Core\Components\CharacterComponents\CharacterEquipmentComponent.h"

UBTTask_Fire::UBTTask_Fire()
{
	NodeName = "Fire";
}

EBTNodeResult::Type UBTTask_Fire::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

	const UCharacterEquipmentComponent* EquipmentComponent = Character->GetCharacterEquipmentComponent();

	ARangeWeaponItem* RangeWeapon = EquipmentComponent->GetCurrentRangeWeapon();

	if (!IsValid(RangeWeapon))
	{
		return EBTNodeResult::Failed;
	}

	AActor* CurrentTarget = Cast<AActor>(Blackboard->GetValueAsObject(TargetKey.SelectedKeyName));


	if (!IsValid(CurrentTarget))
	{
		Character->StopFire();
		return EBTNodeResult::Failed;
	}

	float DistSq = FVector::DistSquared(CurrentTarget->GetActorLocation(), Character->GetActorLocation());

	if (DistSq > FMath::Square(MaxFireDistance))
	{
		Character->StopFire();
		return EBTNodeResult::Failed;
	}

	if (RangeWeapon->IsFiring())
	{
		return EBTNodeResult::Failed;
	}

	Character->StartFire();
	return EBTNodeResult::Succeeded;
}
