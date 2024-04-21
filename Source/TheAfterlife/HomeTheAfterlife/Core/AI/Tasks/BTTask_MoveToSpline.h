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
