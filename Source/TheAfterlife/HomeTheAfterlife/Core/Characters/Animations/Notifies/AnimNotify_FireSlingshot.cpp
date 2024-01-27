// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_FireSlingshot.h"
#include <TheAfterlife/HomeTheAfterlife/Core/Characters/BaseCharacter.h>
#include "../../../Components\CharacterComponents\CharacterEquipmentComponent.h"
#include "../../../Actors\Equipment\Weapons\RangeWeaponItem.h"


void UAnimNotify_FireSlingshot::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	ABaseCharacter* CharacterOwner = Cast<ABaseCharacter>(MeshComp->GetOwner());

	if (!IsValid(CharacterOwner))
	{
		return;
	}

	CharacterOwner->GetCharacterEquipmentComponent_Mutable()->GetCurrentRangeWeapon()->MakeShot();
}

