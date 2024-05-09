#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../../AI/Characters/BaseAICharacter.h"
#include "EnemySpawner.generated.h"

UCLASS()
class THEAFTERLIFE_API AEnemySpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	
	AEnemySpawner();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy")
	TArray<TSubclassOf<ABaseAICharacter>> EnemyTypes;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy")
	int32 AmountOfEnemies;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn Position")
	float Radius = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn Position")
	float MinimumDistanceToPlayer = 300.0f;

	void SpawnEnemies();
	void DestroyEnemies();


private:

	TArray<class ABaseAICharacter*> SpawnedEnemies;
	FVector CalculateSpawnPoint();

};
