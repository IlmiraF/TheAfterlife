// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "State.h"
#include "StateComposite.generated.h"

/**
 * 
 */
UCLASS()
class THEAFTERLIFE_API UStateComposite : public UState
{
	GENERATED_BODY()

public:

	void Enter() override;

	void Exit() override;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "States")
	TArray<UState*> States;
	
};
