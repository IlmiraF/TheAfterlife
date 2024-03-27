// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HealthStealingWidget.generated.h"

/**
 * 
 */
UCLASS()
class THEAFTERLIFE_API UHealthStealingWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent)
	void HealthStealingEffect();
	
};
