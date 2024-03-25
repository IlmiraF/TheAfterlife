// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DialogueWidget.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDialogueVisible, bool, IsVisible);


UCLASS()
class THEAFTERLIFE_API UDialogueWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hints")
	FString Speach;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hints")
	bool bIsVisible;

	UFUNCTION()
	void UpdateDialogue(FString newSpeach);

	UFUNCTION(BlueprintCallable)
	void UpdateVisible(bool Visible);

	UPROPERTY(BlueprintAssignable)
	FOnDialogueVisible OnVisible;
	
};
