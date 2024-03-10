#include "BrainComponent.h"
#include "EnemyPoolObject.h"
#include "../../AI/Controllers/BaseAICharacterController.h"


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

	float RandomAngle;
	float RandomDistanceFromCenter;

	bool bIsTooCloseToPlayer = true;
	bool bIsInsideCircle = true;

	while (bIsTooCloseToPlayer || bIsInsideCircle)
	{
		RandomAngle = FMath::RandRange(0.0f, 2.0f * PI);
		RandomDistanceFromCenter = FMath::RandRange(0.0f, Radius);

		SpawnLocation.X = SpawnLocation.X + RandomDistanceFromCenter * FMath::Cos(RandomAngle);
		SpawnLocation.Y = SpawnLocation.Y + RandomDistanceFromCenter * FMath::Sin(RandomAngle);
		SpawnLocation.Z = SpawnLocation.Z;

		if (FVector::Distance(SpawnLocation, PlayerLocation) >= MinimumDistanceToPlayer)
		{
			bIsTooCloseToPlayer = false;
		}

		if (FVector::DistSquared(SpawnLocation, PlayerLocation) >= Radius * Radius)
		{
			bIsInsideCircle = false;
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
	}

	if (NewEnemy)
	{
		NewEnemy->SetActorLocation(SpawnLocation);
		NewEnemy->SetActorRotation((PlayerLocation - SpawnLocation).Rotation());
		NewEnemy->Revival();

		OnCharacterDeathHandle = NewEnemy->OnCharacterDeath.AddUFunction(this, FName("ReturnEnemy"));
	}
}

void AEnemyPoolObject::ReturnEnemy(ABaseAICharacter* Enemy)
{
    if (Enemy)
    {	
		Enemy->OnCharacterDeath.Remove(OnCharacterDeathHandle);
        FreeEnemys.Enqueue(Enemy);
		SpawnNewEnemy();
		
    }
}
