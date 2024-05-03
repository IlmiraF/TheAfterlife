#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../../Actors/Environment/AAltar.h"
#include "EnemyPoolObject.generated.h"




UCLASS()
class THEAFTERLIFE_API AEnemyPoolObject : public AActor
{
	GENERATED_BODY()

public:

	AEnemyPoolObject();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy")
	TArray<TSubclassOf<ABaseAICharacter>> EnemysType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy")
	int32 AmmountEnemy;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Altar")
	AAltar* Altar;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn Position")
	float Radius = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn Position")
	float MinimumDistanceToPlayer = 300.0f;

	void MakeEnemisVisible();

	void DespawnEnemys();

protected:

	virtual void BeginPlay() override;

private:

	void InitSpawnEnemys();

	UFUNCTION()
	void GetFreeEnemy();

	void SpawnEnemy();

	UFUNCTION()
	void ReturnEnemy(ABaseAICharacter* Enemy);

	FVector CalculatingSpawnPoint();

	TQueue<class ABaseAICharacter*> FreeEnemys;

	FDelegateHandle OnCharacterDeathHandle;

	bool bCanSpawn;

};
