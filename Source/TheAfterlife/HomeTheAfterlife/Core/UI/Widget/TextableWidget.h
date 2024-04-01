// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TextableWidget.generated.h"

/**
 * 
 */
UCLASS()
class THEAFTERLIFE_API UTextableWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION()
	virtual void UpdateWidget(FString Text, bool Visible);

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hints")
	FString Hint;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hints")
	bool bIsVisible;
	
};
