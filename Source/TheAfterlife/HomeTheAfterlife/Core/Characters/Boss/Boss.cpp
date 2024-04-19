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

bool ABoss::AltarsIntact()
{
	return AmountAltars>0;
}

void ABoss::BeginPlay()
{
	Super::BeginPlay();;
	GetMesh()->SetSkeletalMesh(FirstStageBossMesh);

	for (AAltar* Altar : Altars)
	{
		Altar->OnAltarDestroyed.AddUObject(this, &ABoss::DestructionAltars);
	}

	AmountAltars = Altars.Num();

}

void ABoss::DestructionAltars()
{
	AmountAltars--;
}