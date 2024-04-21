#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_MoveToGround.generated.h"


UCLASS()
class THEAFTERLIFE_API UBTTask_MoveToGround : public UBTTaskNode
{
	GENERATED_BODY()

public:

	UBTTask_MoveToGround();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
