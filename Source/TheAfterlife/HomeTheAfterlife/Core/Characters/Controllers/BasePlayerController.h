// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BasePlayerController.generated.h"

/**
 * 
 */

class ABaseCharacter;

UCLASS()
class THEAFTERLIFE_API ABasePlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	virtual void SetPawn(APawn* InPawn) override;

protected:

	virtual void SetupInputComponent() override;

	void MoveForward(float value);
	void MoveRight(float value);
	void Turn(float value);
	void LookUp(float value);
	void Mantle();
	void Jump();
	void ChangeCrouchState();

	void ClimbLadderUp(float value);
	void InteractWithLadder();

	void InteractWithZipline();

	void NextItem();
	void PreviousItem();

	void Fire();

	void PunchAttack();
	void KickAttack();
	//void MeleeAttackStart();
	//void MeleeAttackFinish();

	TSoftObjectPtr<ABaseCharacter> CachedBaseCharacter;
};
