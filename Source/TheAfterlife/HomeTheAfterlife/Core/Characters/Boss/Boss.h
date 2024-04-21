#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/Character.h"
#include "AIController.h"
#include "../../Characters/BaseCharacter.h"
#include "../../Actors/Enemys/EnemyPoolObject.h"
#include "Components/SplineComponent.h"
#include "../../Actors/Environment/AAltar.h"
#include "../../Actors/Navigation/SplineActor.h"
#include "../../Components/CharacterComponents/CharacterAttributeComponent.h"
#include "Boss.generated.h"


DECLARE_MULTICAST_DELEGATE(FOnFirstStageCompleted);
DECLARE_MULTICAST_DELEGATE(FOnMovedToCircleSpline);

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

	bool bIsMovingToSpline = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float FlySpeed = 300.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	ASplineActor* CircleSplineActor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	ASplineActor* RiseSplineActor;

	virtual void Tick(float DeltaTime) override;

	void SetSplineMovement(bool Value);

	void SwitchSplines(EBirdFlinghtTypes FlyType);

	void SetInvulnerable(bool Value);

	FOnFirstStageCompleted OnFirstStageCompleted;
	FOnMovedToCircleSpline OnMovedToCircleSpline;
	
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

	void DestructionAltars();

	void FirstStageCompleted();

	float GetSplineLength();

	void SplineMovement(float DeltaTime);

	float DistanceAlongSpline = 0;

	USplineComponent* CachedSplineComponent;

	EBirdFlinghtTypes CurrentFlyType = EBirdFlinghtTypes::Rise;

	void MovementAlongCompleted();
};