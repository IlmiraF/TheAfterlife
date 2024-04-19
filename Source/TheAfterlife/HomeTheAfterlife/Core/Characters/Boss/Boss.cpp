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

void ABoss::BeginPlay()
{
	Super::BeginPlay();;
	GetMesh()->SetSkeletalMesh(FirstStageBossMesh);

	for (AAltar* Altar : Altars)
	{
		Altar->OnAltarDestroyed.AddUObject(this, &ABoss::TakeDamageFirstStage);
	}

	AmountAltars = Altars.Num();

}

void ABoss::TakeDamageFirstStage()
{
	AmountAltars--;

	if (AmountAltars <= 0)
	{
	}
}
