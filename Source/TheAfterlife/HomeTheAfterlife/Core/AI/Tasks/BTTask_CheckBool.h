#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_CheckBool.generated.h"

/**
 *
 */
UCLASS()
class THEAFTERLIFE_API UBTTask_CheckBool : public UBTTaskNode
{
	GENERATED_BODY()

public:

	UBTTask_CheckBool();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	FBlackboardKeySelector TargetKey;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	bool TargetValue;

};
