// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_CheckHeathPercent.generated.h"

/**
 * 
 */
UCLASS()
class THEAFTERLIFE_API UBTTask_CheckHeathPercent : public UBTTaskNode
{
	GENERATED_BODY()

public:

	UBTTask_CheckHeathPercent();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	float MaxPercent = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	float MinPercent = 0.0f;
	
};
