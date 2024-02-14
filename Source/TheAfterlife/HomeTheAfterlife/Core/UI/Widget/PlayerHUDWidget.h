// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHUDWidget.generated.h"


UCLASS()
class THEAFTERLIFE_API UPlayerHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public :

	class UAmmoWidget* GetWidgetAmmo();

	UFUNCTION()
	void SetBombAmmo(int32 Ammo);

protected:

	UFUNCTION(BlueprintCallable)
	float GetHealthPercent() const;

	UFUNCTION(BlueprintCallable)
	int GetBombAmmo() const;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget names")
	FName AmmoWidgetName;

private: 

	int32 AmmoUI;
};
