// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_CheckPlayerIsAlive.generated.h"

/**
 * 
 */
UCLASS()
class THEAFTERLIFE_API UBTTask_CheckPlayerIsAlive : public UBTTaskNode
{
	GENERATED_BODY()

public:

	UBTTask_CheckPlayerIsAlive();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	FBlackboardKeySelector TargetKey;
	
};
