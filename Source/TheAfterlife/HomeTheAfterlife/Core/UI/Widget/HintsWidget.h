// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TextableWidget.h"
#include "HintsWidget.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHintsVisible, bool, IsVisible);

UCLASS()
class THEAFTERLIFE_API UHintsWidget : public UTextableWidget
{
	GENERATED_BODY()

public:

	virtual void UpdateWidget(FString Text, bool Visible) override;

	UPROPERTY(BlueprintAssignable)
	FOnHintsVisible OnVisible;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Timer = 5.0f;

private:

	FTimerHandle VisibilityTimer;
	void DisableHint();
};
