// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class THEAFTERLIFE_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct() override;

	void LoadMainMenu();

	UFUNCTION(BlueprintCallable)
	void StartGame();

	void ShowSettings();

	void ExitGame();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName LobbyMapName;

	TSoftObjectPtr<APlayerController> PlayerController;
};
