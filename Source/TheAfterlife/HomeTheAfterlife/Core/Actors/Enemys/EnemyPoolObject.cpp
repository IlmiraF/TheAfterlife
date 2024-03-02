
#include "EnemyPoolObject.h"


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
	for (int32 i = 0; i < EnemysArray.Num(); i++)
	{	
		FActorSpawnParameters SpawnParams;


		FTransform SpawnTransform = this->GetActorTransform();
		FVector SpawnPoint = SpawnTransform.TransformPosition(InitSpawnPoint + FVector(DistanceBetweenRows * CurrentRows*-1, DistanceBetweenCols * CurrentCols, 0));

		ABaseAICharacter* Enemy = GetWorld()->SpawnActor<ABaseAICharacter>(EnemysArray[i], SpawnPoint, FRotator::ZeroRotator);

		CurrentCols++;

		if (CurrentCols == Cols)
		{
			CurrentCols = 0;
			CurrentRows++;
		}
	}
}