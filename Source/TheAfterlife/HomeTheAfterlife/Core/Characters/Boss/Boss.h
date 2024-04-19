
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/Character.h"
#include "AIController.h"
#include "../../Characters/BaseCharacter.h"
#include "../../Actors/Enemys/EnemyPoolObject.h"
#include "Components/SplineComponent.h"
#include "../../Actors/Environment/AAltar.h"
#include "States/StateMachine.h"
#include "States/StateResolver.h"
#include "States/BirdState.h"
#include "States/FlyUPState.h"
#include "Boss.generated.h"



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
	USkeletalMesh* FirstStageBossMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	USkeletalMesh* SecondStageBossMesh;	

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	UBehaviorTree* BehaviorTree;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Altars")
	TArray<AAltar*> Altars;

private:

	int32 AmountAltars;

	USkeletalMeshComponent* CachedSkeletalMesh;

	void TakeDamageFirstStage();
};