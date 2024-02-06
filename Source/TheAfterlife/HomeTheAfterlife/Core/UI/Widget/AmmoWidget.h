// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AmmoWidget.generated.h"

/**
 * 
 */
UCLASS()
class THEAFTERLIFE_API UAmmoWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ammo")
	int32 Ammo;

	UFUNCTION(BlueprintCallable)
	int32 GetAmmoCount() const;

public:

	UFUNCTION()
	void UpdateAmmoCount(int32 NewAmmo);
};
