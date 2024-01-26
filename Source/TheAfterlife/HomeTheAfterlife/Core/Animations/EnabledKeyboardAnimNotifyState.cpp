// Fill out your copyright notice in the Description page of Project Settings.

#include "../../Core\Characters\BaseCharacter.h"
#include "EnabledKeyboardAnimNotifyState.h"

void UEnabledKeyboardAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{	
	if (MeshComp == NULL || MeshComp->GetOwner() == NULL)
	{
		return;
	}

	ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(MeshComp->GetOwner());

	if (BaseCharacter != NULL)
	{
		BaseCharacter->SetCanMove(false);
	}
}

void UEnabledKeyboardAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{	
	if (MeshComp == NULL || MeshComp->GetOwner() == NULL)
	{
		return;
	}

	ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(MeshComp->GetOwner());

	if (BaseCharacter != NULL)
	{
		BaseCharacter->SetCanMove(true);
	}
}
