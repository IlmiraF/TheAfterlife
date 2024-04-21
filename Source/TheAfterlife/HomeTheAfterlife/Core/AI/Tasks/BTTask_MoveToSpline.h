// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_MoveToSpline.generated.h"

/**
 * 
 */
UCLASS()
class THEAFTERLIFE_API UBTTask_MoveToSpline : public UBTTaskNode
{
	GENERATED_BODY()

public:

	UBTTask_MoveToSpline();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
