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

	void InteractWithRunWall();

	void ClimbMoveForward(float value);
	void ClimbMoveRight(float value);
	void ClimbHop();
	void OnClimbActionStarted();

	void OnBeamMoveForward(float value);
	void OnBeamMoveRight(float value);

	void QuickSaveGame();
	void QuickLoadGame();

	TSoftObjectPtr<ABaseCharacter> CachedBaseCharacter;
};
