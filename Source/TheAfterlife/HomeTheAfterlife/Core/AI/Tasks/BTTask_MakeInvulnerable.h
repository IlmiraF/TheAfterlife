#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_MakeInvulnerable.generated.h"


UCLASS()
class THEAFTERLIFE_API UBTTask_MakeInvulnerable : public UBTTaskNode
{
	GENERATED_BODY()

public:

	UBTTask_MakeInvulnerable();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
