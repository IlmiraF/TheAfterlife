#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BaseAIController.h"
#include "AITurretController.generated.h"



UCLASS()
class THEAFTERLIFE_API AAITurretController : public ABaseAIController
{
	GENERATED_BODY()

public:

	virtual void SetPawn(APawn* InPawn) override;

	virtual void ActorsPerceptionUpdated(const TArray<AActor*>& UpdatedActors) override;

private:

	TWeakObjectPtr<class ATurret> CachedTurret;
	
};
