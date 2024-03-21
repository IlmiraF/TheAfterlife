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

	UFUNCTION(BlueprintCallable)
	void StartGame();

	UFUNCTION(BlueprintCallable)
	void LoadSaveGame();

	UFUNCTION(BlueprintCallable)
	void GetSettings();

	UFUNCTION(BlueprintCallable)
	void ExitGame();
	

protected:

	UPROPERTY(EditDefaultsOnly)
	FName LevelName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widgets")
	TSubclassOf<class USettingsWidget> SettingsWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widgets")
	TSubclassOf<class USavesWidget> SavesWidgetClass;
};
