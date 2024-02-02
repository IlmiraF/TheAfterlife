// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HintsWidget.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnVisible);

UCLASS()
class THEAFTERLIFE_API UHintsWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hints")
	FString Hint;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hints")
	bool bIsVisible;

	UFUNCTION()
	void UpdateHint(FString newHint);

	UFUNCTION(BlueprintCallable)
	bool UpdateVisible(bool Visible);

	UPROPERTY(BlueprintAssignable)
	FOnVisible OnVisibleKurwa;

};
