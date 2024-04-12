#include "Boss.h"

ABoss::ABoss(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
}

UBehaviorTree* ABoss::GetBehaviorTree() const
{
	return BehaviorTree;
}

void ABoss::SpawnEnemy()
{
	EnemyPoolObject->MakeEnemisVisible();
}

//void ABoss::SplineMovement()
//{
//	if (!SplineComponent)
//	{
//		return;
//	}
//
//	DistanceAlongSpline += FlySpeed;
//
//	FVector CurrentSplineLocation = SplineComponent->GetLocationAtDistanceAlongSpline(DistanceAlongSpline, ESplineCoordinateSpace::World);
//	FRotator CurrentSplineRotation = SplineComponent->GetRotationAtDistanceAlongSpline(DistanceAlongSpline, ESplineCoordinateSpace::World);
//	CurrentSplineRotation = CurrentSplineRotation - FRotator(0.f, 90.0f, 0.f);
//
//	GetMesh()->SetWorldLocationAndRotation(CurrentSplineLocation, CurrentSplineRotation);
//}

void ABoss::BeginPlay()
{
	Super::BeginPlay();

	//CachedSkeletalMesh = GetMesh();
	GetMesh()->SetSkeletalMesh(FirstStageBossMesh);

	for (AAltar* Altar : Altars)
	{
		Altar->OnAltarDestroyed.AddUObject(this, &ABoss::TakeDamageFirstStage);
	}

	FirstStageHealth = Altars.Num();
}

void ABoss::TakeDamageFirstStage()
{
	FirstStageHealth--;
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, TEXT("Ox Blya"));

	if (FirstStageHealth <= 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("Trash"));
	}
}
