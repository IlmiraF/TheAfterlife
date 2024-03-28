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

	void NextItem();
	void PreviousItem();

	void Fire();

	void StartAiming();
	void StopAiming();

	void EquipPrimaryItem();

	void HandsMeleeAttack();
	void LegsMeleeAttack();

	void ThrowBomb();

	TSoftObjectPtr<ABaseCharacter> CachedBaseCharacter;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widgets")
	TSubclassOf<class UPlayerHUDWidget> PlayerHUDWidgetClass;

	void Interact();

private:

	void CreateAndInitializeWidgets();

	void OnInteractableObjectFound(bool bIsVisible, FName KeyName);

	class UPlayerHUDWidget* PlayerHUDWidget = nullptr;

	bool bIgnoreCameraPitch = false;

	void UpdateHintsWidget(FString TutorialText, bool Visibility);

	//void ToggleMainMenu();
};
