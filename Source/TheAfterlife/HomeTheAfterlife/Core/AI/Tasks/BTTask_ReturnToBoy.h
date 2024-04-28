#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_ReturnToBoy.generated.h"


UCLASS()
class THEAFTERLIFE_API UBTTask_ReturnToBoy : public UBTTaskNode
{
	GENERATED_BODY()

public:

	UBTTask_ReturnToBoy();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
