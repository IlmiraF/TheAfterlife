// Fill out your copyright notice in the Description page of Project Settings.

#include "../../../../../TheAfterlifeTypes.h"
#include "AnimNotify_SetVisibility.h"
#include <TheAfterlife/HomeTheAfterlife/Core/AI/Characters/BaseAICharacter.h>

void UAnimNotify_SetVisibility::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	MeshComp->SetVisibility(false);
	AActor* Actor = MeshComp->GetOwner();
	Actor->SetActorEnableCollision(false);
	Actor->SetActorTickEnabled(false);
	ABaseAICharacter* AICharacter = Cast<ABaseAICharacter>(Actor);

	AICharacter->DisableCharacter();
}

