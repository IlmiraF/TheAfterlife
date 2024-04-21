#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_MakeVulnerable.generated.h"


UCLASS()
class THEAFTERLIFE_API UBTTask_MakeVulnerable : public UBTTaskNode
{
	GENERATED_BODY()
	
public:

	UBTTask_MakeVulnerable();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
