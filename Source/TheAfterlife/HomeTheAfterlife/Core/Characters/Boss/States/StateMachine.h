// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../Source/TheAfterlife/TheAfterlifeTypes.h"
#include "State.h"
#include "StateMachine.generated.h"


USTRUCT(BlueprintType)
struct FStateHolder
{	
	GENERATED_BODY()

	EBossStateType Type;
	UState* State;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class THEAFTERLIFE_API UStateMachine : public UState
{
	GENERATED_BODY()

public:	

	void Enter() override;

	void Exit() override;

	void SwitchState(EBossStateType StateType);

	void AddState(EBossStateType Type, UState* State);

protected:

	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "States")
	bool EnterOnEnable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "States")
	bool ExitOnDisable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "States")
	TArray<FStateHolder> States;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "States")
	EBossStateType CurrentStateType;

private:

	UState* CurrentState;

	UState* FindState(EBossStateType StateType);
};