#include "BrainComponent.h"
#include "EnemyPoolObject.h"
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

void AEnemyPoolObject::SpawnNewEnemy()
{   
    FVector SpawnLocation = this->GetActorLocation();
	FVector PlayerLocation = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();

    float RandomAngle;

    bool bIsTooCloseToPlayer = true;
    bool bIsInsideCircle = true;

    while (bIsTooCloseToPlayer || bIsInsideCircle)
    {
        RandomAngle = FMath::RandRange(0.0f, 2.0f * PI);

        SpawnLocation.X = SpawnLocation.X + Radius * FMath::Cos(RandomAngle);
        SpawnLocation.Y = SpawnLocation.Y + Radius * FMath::Sin(RandomAngle);
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

	int RandomEnemyTypeIndex = FMath::RandRange(0, 1);

    FActorSpawnParameters SpawnParams;
    ABaseAICharacter* Enemy = GetWorld()->SpawnActor<ABaseAICharacter>(EnemysType[RandomEnemyTypeIndex], SpawnLocation, FRotator::ZeroRotator);
    Enemy->SetActorRotation((PlayerLocation - SpawnLocation).Rotation());

    //Enemy->OnCharacterDeath.AddUFunction(this, FName("SpawnNewEnemy"));
}

