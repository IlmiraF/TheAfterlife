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
	for (int32 i = 0; i < InitEnemysArray.Num(); i++)
	{	
		FActorSpawnParameters SpawnParams;


		FTransform SpawnTransform = this->GetActorTransform();
		FVector SpawnPoint = SpawnTransform.TransformPosition(InitSpawnPoint + FVector(DistanceBetweenRows * CurrentRows*-1, DistanceBetweenCols * CurrentCols, 0));

		ABaseAICharacter* Enemy = GetWorld()->SpawnActor<ABaseAICharacter>(InitEnemysArray[i], SpawnPoint, FRotator::ZeroRotator);
		EnemysArray.Add(Enemy);

		CurrentCols++;

		if (CurrentCols == Cols)
		{
			CurrentCols = 0;
			CurrentRows++;
		}
	}

	for (int32 i = Cols; i < InitEnemysArray.Num(); i++)
	{
		ABaseAICharacterController* AICharacterController = EnemysArray[i]->GetController<ABaseAICharacterController>();
		
		UBrainComponent* Brain = AICharacterController->GetBrainComponent();
		Brain->StopLogic("Disable");
	}

	//for (int32 i = Cols; i < InitEnemysArray.Num(); i++)
	//{
	//	ABaseAICharacterController* AICharacterController = EnemysArray[i]->GetController<ABaseAICharacterController>();
	//
	//	UBrainComponent* Brain = AICharacterController->GetBrainComponent();
	//	Brain->RestartLogic();
	//}
}