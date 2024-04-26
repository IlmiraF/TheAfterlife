#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_CheckCanThrow.generated.h"

UCLASS()
class THEAFTERLIFE_API UBTTask_CheckCanThrow : public UBTTaskNode
{
	GENERATED_BODY()

public:

	UBTTask_CheckCanThrow();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
