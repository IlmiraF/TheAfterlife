// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class THEAFTERLIFE_API UPlayerHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	class UHintsWidget* GetHintsWidget();

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget names")
	FName HintsWidgetName;

	UFUNCTION(BlueprintCallable)
	float GetHealthPercent() const;

	UFUNCTION(BlueprintCallable)
	float GetLeftBalancePercent() const;

	UFUNCTION(BlueprintCallable)
	float GetRightBalancePercent() const;
	
};
