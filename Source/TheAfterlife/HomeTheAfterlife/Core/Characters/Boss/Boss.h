
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/Character.h"
#include "AIController.h"
#include "../../Characters/BaseCharacter.h"
#include "../../Actors/Enemys/EnemyPoolObject.h"
#include "Boss.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnDoorsOpening);

UCLASS()
class THEAFTERLIFE_API ABoss : public ABaseCharacter
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	AEnemyPoolObject* EnemyPoolObject;

	ABoss(const FObjectInitializer& ObjectInitializer);

	UBehaviorTree* GetBehaviorTree() const;

	void SpawnEnemy();
	
protected:

	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	USkeletalMeshComponent* FirstStageBossMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	USkeletalMeshComponent* SecondStageBossMesh;	

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	UBehaviorTree* BehaviorTree;
};