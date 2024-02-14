#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "TheAfterlife\HomeTheAfterlife\Core\AI\Characters\Turret.h"
#include "AITurretController.generated.h"



UCLASS()
class THEAFTERLIFE_API AAITurretController : public AAIController
{
	GENERATED_BODY()

public:

	AAITurretController();

	virtual void SetPawn(APawn* InPawn) override;

	virtual void ActorsPerceptionUpdated(const TArray<AActor*>& UpdatedActors) override;

private:

	TWeakObjectPtr<ATurret> CachedTurret;
	
};
