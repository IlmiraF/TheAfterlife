// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_MeleeAttack.generated.h"

/**
 * 
 */
UCLASS()
class THEAFTERLIFE_API UBTService_MeleeAttack : public UBTService
{
	GENERATED_BODY()
	

public:

	UBTService_MeleeAttack();

protected:

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	FBlackboardKeySelector TargetKey;
};
