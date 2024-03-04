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
		Enemy->SetIndex(i);
		Enemy->OnCharacterDeath.AddUFunction(this, FName("SwitchPosition"));
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
}

void AEnemyPoolObject::SwitchPosition(int32 Index)
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("Character Death Index: %d"), Index));

	ABaseAICharacter* DeathEnemy = EnemysArray[Index];

	for (int32 i = Cols+Index; i < EnemysArray.Num(); i+=Cols)
	{	
		ABaseAICharacter* Enemy = EnemysArray[i]; 
		Enemy->SetIndex(i-Cols);
		EnemysArray[i - Cols] = Enemy;

		if (i == Cols + Index)
		{
			ABaseAICharacterController* AICharacterController = Enemy->GetController<ABaseAICharacterController>();

			UBrainComponent* Brain = AICharacterController->GetBrainComponent();
			Brain->RestartLogic();
		}
	}

	int32 LastIndex = EnemysArray.Num() - Cols + Index;
															 // переприсваем второй ряд на первый, и когда умирает второй ряд, мы обращаемся по индексам второго ряда а там уже пусто, надо сдвигать всех на индексы ряда
	DeathEnemy->SetIndex(LastIndex);
	EnemysArray[LastIndex]= DeathEnemy;
	
	ABaseAICharacterController* AICharacterController = DeathEnemy->GetController<ABaseAICharacterController>();

	UBrainComponent* Brain = AICharacterController->GetBrainComponent();
	Brain->StopLogic("Disable");
}
