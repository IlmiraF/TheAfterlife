// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "TheAfterlife\TheAfterlifeTypes.h"
#include "..\..\Components\CharacterComponents\CharacterEquipmentComponent.h"
#include "BTTask_EquipItem.generated.h"

/**
 * 
 */
UCLASS()
class THEAFTERLIFE_API UBTTask_EquipItem : public UBTTaskNode
{
	GENERATED_BODY()

public:

	UBTTask_EquipItem();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	EEquipmentSlots EquipItem;
	
};
