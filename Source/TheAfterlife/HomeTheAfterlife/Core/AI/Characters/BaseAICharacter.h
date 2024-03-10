#pragma once

#include "CoreMinimal.h"
#include "..\..\Characters\BaseCharacter.h"
#include "..\..\Components\CharacterComponents/AIPatrollingComponent.h"
#include "BaseAICharacter.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnCharacterDeath, ABaseAICharacter*);

UCLASS()
class THEAFTERLIFE_API ABaseAICharacter : public ABaseCharacter
{
	GENERATED_BODY()
	
public:

	ABaseAICharacter(const FObjectInitializer& ObjectInitializer);

	UAIPatrollingComponent* GetPatrollingComponent() const;

	UBehaviorTree* GetBehaviorTree() const;

	virtual void OnDeath() override;

	FOnCharacterDeath OnCharacterDeath;

	void DisableCharacter();

	void Revival();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UAIPatrollingComponent* AIPatrollingComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	UBehaviorTree* BehaviorTree;

};
