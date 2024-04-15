// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../Source/TheAfterlife/TheAfterlifeTypes.h"
#include "StateMachine.generated.h"

USTRUCT(BlueprintType)
struct FStateHolder
{	
	GENERATED_BODY()

	EBossStateType Type;
	//UState State;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class THEAFTERLIFE_API UStateMachine : public UActorComponent
{
	GENERATED_BODY()

public:	
	UStateMachine();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:

	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "States")
	bool EnterOnEnable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "States")
	bool ExitOnDisable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "States")
	TArray<FStateHolder> States;

		
};
