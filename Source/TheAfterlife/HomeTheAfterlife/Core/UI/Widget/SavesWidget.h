// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SavesWidget.generated.h"

/**
 * 
 */
UCLASS()
class THEAFTERLIFE_API USavesWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	void LoadSave(int32 NumOfSave);

	UFUNCTION(BlueprintCallable)
	void BackToMenu();

protected:

	UFUNCTION(BlueprintPure)
	FText SetText() const;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widgets")
	TSubclassOf<class UMainMenuWidget> MainMenuWidgetClass;

private:

	bool CheckExist() const;
};