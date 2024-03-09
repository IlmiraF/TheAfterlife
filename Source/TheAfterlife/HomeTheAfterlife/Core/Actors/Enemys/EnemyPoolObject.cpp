#include "BrainComponent.h"
#include "EnemyPoolObject.h"
#include "NavigationSystem.h"
#include "TheAfterlife/HomeTheAfterlife/Core/AI/Controllers/BaseAICharacterController.h"


AEnemyPoolObject::AEnemyPoolObject()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AEnemyPoolObject::BeginPlay()
{
	Super::BeginPlay();
	
	InitSpawnEnemys();
}

void AEnemyPoolObject::InitSpawnEnemys()
{	
	for (int32 i = 0; i < AmmountEnemy; i++)
	{	
        SpawnNewEnemy();
	}
}

FVector AEnemyPoolObject::CalculatingSpawnPoint()
{
    FVector SpawnLocation = this->GetActorLocation();
    FVector PlayerLocation = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();

    UNavigationSystemV1* NavSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
    if (NavSystem)
    {
        FNavLocation NavLocation;
        if (NavSystem->GetRandomPointInNavigableRadius(SpawnLocation, Radius, NavLocation))
        {
            SpawnLocation = NavLocation.Location;
        }
    }

    return SpawnLocation;
}

void AEnemyPoolObject::SpawnNewEnemy()
{   
	FVector PlayerLocation = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
	FVector SpawnLocation = CalculatingSpawnPoint();

	ABaseAICharacter* NewEnemy = nullptr;

	if (!FreeEnemys.IsEmpty())
	{
		FreeEnemys.Dequeue(NewEnemy);
	}
	else
	{
		int32 RandomIndex = FMath::RandRange(0, EnemysType.Num() - 1);
		NewEnemy = GetWorld()->SpawnActor<ABaseAICharacter>(EnemysType[RandomIndex]);
		//NewEnemy->OnCharacterDeath.AddUFunction(this, FName("ReturnEnemy"));
	}

	if (NewEnemy)
	{
		NewEnemy->SetActorHiddenInGame(false);
		NewEnemy->SetActorEnableCollision(true);
		NewEnemy->SetActorTickEnabled(true);
		NewEnemy->SetActorLocation(SpawnLocation);
		NewEnemy->SetActorRotation((PlayerLocation - SpawnLocation).Rotation());
		//NewEnemy->Revival();

		//NewEnemy->SetEnemyPool(this);
	}
}

void AEnemyPoolObject::ReturnEnemy(ABaseAICharacter* Enemy)
{
    if (Enemy)
    {
        //Enemy->SetActorHiddenInGame(true);
        //Enemy->SetActorEnableCollision(false);
        //Enemy->SetActorTickEnabled(false);
        FreeEnemys.Enqueue(Enemy);
		SpawnNewEnemy();
    }
}

// Enemy->OnCharacterDeath.AddUFunction(this, FName("AddEnemy"));