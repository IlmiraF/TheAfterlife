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
DECLARE_MULTICAST_DELEGATE(FOnPhaseIsOver);
DECLARE_MULTICAST_DELEGATE(FOnBossIsDead);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnMovedToCircleSpline, bool);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnBossConcussed, bool);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnBossHasLanded, bool);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnBossIsBoy, bool);


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

	void SetMovementToGround(bool Value);

	void SwitchSplines(EBirdFlinghtTypes FlyType);

	void SetInvulnerable(bool Value);

	void Concussion();

	bool GetSecondStage();

	void StartSecondStage();

	void ReturnToBoy();

	FOnFirstStageCompleted OnFirstStageCompleted;

	FOnMovedToCircleSpline OnMovedToCircleSpline;

	FOnBossConcussed OnBossConcussed;

	FOnBossHasLanded OnBossHasLanded;

	FOnBossIsBoy OnBossIsBoy;

	FOnPhaseIsOver OnPhaseIsOver;

	FOnBossIsDead OnBossIsDead;
	
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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Altars")
	int AmountFirstStageAltars;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	FVector BossLocation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Concussion")
	float TimeConcussion;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Concussion")
	float ConcussionDamage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Boosters")
	float BoosterMeleeDamage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Boosters")
	float BoosterBombDamage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Boosters")
	float BoosterBulletDamage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Boosters")
	float ShootingAccuracyBooster;

private:

	int32 AmountAltars;

	void DestructionAltars();

	void FirstStageCompleted();

	float GetSplineLength();

	void SplineMovement(float DeltaTime);

	void MoveToBossLocation(float DeltaTime);

	float DistanceAlongSpline = 0;

	USplineComponent* CachedSplineComponent;

	EBirdFlinghtTypes CurrentFlyType = EBirdFlinghtTypes::Rise;

	void MovementAlongCompleted();

	bool bOnGround = false;

	bool bOnConcussed;

	void BossConcussed();

	FTimerHandle ConcussionTimerHandle;

	void ReturnToBird();

	bool bIsConcussionTimerRunning = false;

	void ChangeHealth(float newHealthPercent);

	bool bFirstStageCompleted = false;

	bool FirstPhase = true;
	bool SecondPhase = false;
	bool ThirdPhase = false;

	void NewPhase();

	void BoosterSelection();

	bool bSecondStage = false;

	void ReturnToBoyState();
};