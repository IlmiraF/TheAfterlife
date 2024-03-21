// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_StartAttackSpikeTrap.h"
#include "TheAfterlife/HomeTheAfterlife/Core/Actors/Environment/SpikeTrap.h"

void UAnimNotify_StartAttackSpikeTrap::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	ASpikeTrap* SpikeTrap= Cast<ASpikeTrap>(MeshComp->GetOwner());


	if (IsValid(SpikeTrap))
	{
		SpikeTrap->SetCanAttack(true);
	}

}
