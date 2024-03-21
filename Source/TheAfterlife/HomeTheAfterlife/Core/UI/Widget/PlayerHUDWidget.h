// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHUDWidget.generated.h"

class UAmmoWidget;
class UHintsWidget;
UCLASS()
class THEAFTERLIFE_API UPlayerHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public :

	UAmmoWidget* GetWidgetAmmo();

	UFUNCTION()
	void SetBombAmmo(int32 Ammo);

	UHintsWidget* GetHintsWidget();

protected:

	UFUNCTION(BlueprintCallable)
	float GetHealthPercent() const;

	UFUNCTION(BlueprintCallable)
	int GetBombAmmo() const;

	UFUNCTION(BlueprintCallable)
	float GetLeftBalancePercent() const;

	UFUNCTION(BlueprintCallable)
	float GetRightBalancePercent() const;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget names")
	FName AmmoWidgetName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget names")
	FName HintsWidgetName;

private: 

	int32 AmmoUI;
};
