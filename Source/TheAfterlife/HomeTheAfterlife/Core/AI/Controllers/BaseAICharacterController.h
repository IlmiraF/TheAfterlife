
#pragma once

#include "CoreMinimal.h"
#include "BaseAIController.h"
#include "BaseAICharacterController.generated.h"

class ABaseAICharacter;
UCLASS()
class THEAFTERLIFE_API ABaseAICharacterController : public ABaseAIController
{
	GENERATED_BODY()

public:

	virtual void SetPawn(APawn* InPawn) override;

	virtual void OnPossess(APawn* InPawn) override;


	virtual void ActorsPerceptionUpdated(const TArray<AActor*>& UpdatedActors) override;

	virtual void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;

protected:

	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
	float TargetReachRadius = 100.0f;

private:

	void TryMoveToNextTarget();

	bool IsTargetReached(FVector TargetLocation) const;

	TWeakObjectPtr<ABaseAICharacter> CachedAICharacter;

	bool bIsPatrolling;
	
};
