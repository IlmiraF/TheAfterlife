
#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_SplineMovement.generated.h"


UCLASS()
class THEAFTERLIFE_API UBTTask_SplineMovement : public UBTTaskNode
{
	GENERATED_BODY()

public:

	UBTTask_SplineMovement();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
