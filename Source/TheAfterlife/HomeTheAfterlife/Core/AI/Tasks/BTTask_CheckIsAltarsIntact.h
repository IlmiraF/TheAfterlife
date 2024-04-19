#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_CheckIsAltarsIntact.generated.h"

/**
 * 
 */
UCLASS()
class THEAFTERLIFE_API UBTTask_CheckIsAltarsIntact : public UBTTaskNode
{
	GENERATED_BODY()

public:

	UBTTask_CheckIsAltarsIntact();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
