
#include "EnemySpawner.h"
#include "../../AI/Characters/BaseAICharacter.h"
#include "../../AI/Controllers/BaseAICharacterController.h"

AEnemySpawner::AEnemySpawner()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AEnemySpawner::SpawnEnemies()
{
    for (int32 i = 0; i < AmountOfEnemies; ++i)
    {
        FVector SpawnLocation = CalculateSpawnPoint();
        FRotator SpawnRotation = FRotator::ZeroRotator;

        ABaseAICharacter* NewEnemy = GetWorld()->SpawnActor<ABaseAICharacter>(EnemyTypes[FMath::RandRange(0, EnemyTypes.Num() - 1)], SpawnLocation, SpawnRotation);

        if (NewEnemy)
        {
            SpawnedEnemies.Add(NewEnemy);
        }
    }
}

void AEnemySpawner::DestroyEnemies()
{
    for (ABaseAICharacter* Enemy : SpawnedEnemies)
    {
        if (Enemy)
        {	
			Enemy->DisableCharacter();
            Enemy->Destroy();
        }
    }

    SpawnedEnemies.Empty();
}

FVector AEnemySpawner::CalculateSpawnPoint()
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



