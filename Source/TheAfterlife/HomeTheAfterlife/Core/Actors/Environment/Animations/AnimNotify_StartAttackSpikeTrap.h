// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_StartAttackSpikeTrap.generated.h"

/**
 * 
 */
UCLASS()
class THEAFTERLIFE_API UAnimNotify_StartAttackSpikeTrap : public UAnimNotify
{
	GENERATED_BODY()

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
	
};
