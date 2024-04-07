#include "Boss.h"

ABoss::ABoss(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;

	FirstStageBossMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FirstStageBossMesh"));
	FirstStageBossMesh->SetupAttachment(GetRootComponent());

	SecondStageBossMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SecondStageBossMesh"));
	SecondStageBossMesh->SetupAttachment(GetRootComponent());
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
	Super::BeginPlay();
}