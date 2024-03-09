// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SettingsWidget.generated.h"

/**
 * 
 */
UCLASS()
class THEAFTERLIFE_API USettingsWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable)
	void BackToMenu();

	UFUNCTION(BlueprintCallable)
	void SavesMenu();

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widgets")
	TSubclassOf<class UMainMenuWidget> MainMenuWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widgets")
	TSubclassOf<class USavesWidget> SavesWidgetClass;
};
