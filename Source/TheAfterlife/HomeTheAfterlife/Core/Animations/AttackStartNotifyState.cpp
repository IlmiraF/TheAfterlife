#include "AttackStartNotifyState.h"
#include "../../Core\Characters\BaseCharacter.h"
#include "Engine.h"
#include <TheAfterlife/HomeTheAfterlife/Core/Actors/Equipment/Weapons/MeleeWeaponItem.h>
#include "../../Core\Components\CharacterComponents\CharacterEquipmentComponent.h"

void UAttackStartNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{

	if (MeshComp == NULL || MeshComp->GetOwner() == NULL)
	{	
		return;
	}

	//ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(MeshComp->GetOwner());
	//
	//if (BaseCharacter != NULL)
	//{
	//	BaseCharacter->MeleeAttackStart();
	//}

	ABaseCharacter* CharacterOwner = Cast<ABaseCharacter>(MeshComp->GetOwner());
	if (!IsValid(CharacterOwner))
	{
		return;
	}

	AMeleeWeaponItem* MeleeWeapon = CharacterOwner->GetCharacterEquipmentComponent()->GetCurrentMeleeWeaponItem();
	if (IsValid(MeleeWeapon))
	{
		MeleeWeapon->SetIsHitRegistrationEnabled(true);
	}
}

void UAttackStartNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{

	if (MeshComp == NULL || MeshComp->GetOwner() == NULL)
	{
		return;
	}

	//ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(MeshComp->GetOwner());
	//
	//if (BaseCharacter != NULL)
	//{
	//	BaseCharacter->MeleeAttackFinish();
	//}

	ABaseCharacter* CharacterOwner = Cast<ABaseCharacter>(MeshComp->GetOwner());
	if (!IsValid(CharacterOwner))
	{
		return;
	}

	AMeleeWeaponItem* MeleeWeapon = CharacterOwner->GetCharacterEquipmentComponent()->GetCurrentMeleeWeaponItem();
	if (IsValid(MeleeWeapon))
	{
		MeleeWeapon->SetIsHitRegistrationEnabled(false);
	}
}
