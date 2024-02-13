#include "AttackStartNotifyState.h"
#include "Engine.h"
#include "../../BaseCharacter.h"
#include "../../../Actors/Equipment/Weapons/MeleeWeaponItem.h"
#include "../../../Components/CharacterComponents/CharacterEquipmentComponent.h"

void UAttackStartNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{

	if (MeshComp == NULL || MeshComp->GetOwner() == NULL)
	{	
		return;
	}

	ABaseCharacter* CharacterOwner = Cast<ABaseCharacter>(MeshComp->GetOwner());
	if (!IsValid(CharacterOwner))
	{
		return;
	}

	AMeleeWeaponItem* MeleeWeapon = CharacterOwner->GetCharacterEquipmentComponent()->GetCurrentMeleeWeaponItem();
	if (IsValid(MeleeWeapon))
	{
		MeleeWeapon->SetIsHitRegistrationEnabled(true);
		CharacterOwner->MeleeAttackStart();
	}
}

void UAttackStartNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{

	if (MeshComp == NULL || MeshComp->GetOwner() == NULL)
	{
		return;
	}

	ABaseCharacter* CharacterOwner = Cast<ABaseCharacter>(MeshComp->GetOwner());
	if (!IsValid(CharacterOwner))
	{
		return;
	}

	AMeleeWeaponItem* MeleeWeapon = CharacterOwner->GetCharacterEquipmentComponent()->GetCurrentMeleeWeaponItem();
	if (IsValid(MeleeWeapon))
	{
		MeleeWeapon->SetIsHitRegistrationEnabled(false);
		CharacterOwner->MeleeAttackFinish();
	}
}
