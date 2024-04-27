#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_RestoreHealth.generated.h"

UCLASS()
class THEAFTERLIFE_API UBTTask_RestoreHealth : public UBTTaskNode
{
	GENERATED_BODY()
	
public:

	UBTTask_RestoreHealth();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
