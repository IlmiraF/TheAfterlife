// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_Concussion.generated.h"

/**
 * 
 */
UCLASS()
class THEAFTERLIFE_API UBTTask_Concussion : public UBTTaskNode
{
	GENERATED_BODY()

public:

	UBTTask_Concussion();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
