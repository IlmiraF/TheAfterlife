// Fill out your copyright notice in the Description page of Project Settings.
#include "BTTask_MeleeAttack.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "../../Characters/BaseCharacter.h"
#include "../../Components\CharacterComponents\CharacterEquipmentComponent.h"
#include "../../Actors/Equipment/Weapons/MeleeWeaponItem.h"

UBTTask_MeleeAttack::UBTTask_MeleeAttack()
{
	NodeName = "MeleeAttackTask";
}

EBTNodeResult::Type UBTTask_MeleeAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

	AMeleeWeaponItem* MeleeWeapon = EquipmentComponent->GetCurrentMeleeWeaponItem();

	if (!IsValid(MeleeWeapon))
	{
		return EBTNodeResult::Failed;
	}

	AActor* CurrentTarget = Cast<AActor>(Blackboard->GetValueAsObject(TargetKey.SelectedKeyName));


	if (!IsValid(CurrentTarget))
	{
		return EBTNodeResult::Failed;
	}

	float DistSq = FVector::DistSquared(CurrentTarget->GetActorLocation(), Character->GetActorLocation());

	if (DistSq > FMath::Square(RadiusAttack))
	{
		return EBTNodeResult::Failed;
	}

	if (MeleeWeapon->IsAttacking())
	{
		return EBTNodeResult::Failed;
	}

	Character->HandsMeleeAttack();
	return EBTNodeResult::Succeeded;
}