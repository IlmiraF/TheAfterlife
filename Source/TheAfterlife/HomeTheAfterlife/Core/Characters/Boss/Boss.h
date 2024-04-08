
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/Character.h"
#include "AIController.h"
#include "../../Characters/BaseCharacter.h"
#include "../../Actors/Enemys/EnemyPoolObject.h"
#include "Components/SplineComponent.h"
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

	//void SplineMovement();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spline", meta = (MakeEditWidget))
	USplineComponent* SplineComponent;
	
protected:

	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	USkeletalMesh* FirstStageBossMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	USkeletalMesh* SecondStageBossMesh;	

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	UBehaviorTree* BehaviorTree;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spline")
	float DistanceAlongSpline;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	float FlySpeed;

	USkeletalMeshComponent* CachedSkeletalMesh;
};