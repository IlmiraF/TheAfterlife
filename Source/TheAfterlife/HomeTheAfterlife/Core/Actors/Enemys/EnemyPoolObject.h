// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "..\..\..\Core\AI\Characters\BaseAICharacter.h"
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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn Position")
	float Radius = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn Position")
	float MinimumDistanceToPlayer = 300.0f;

protected:

	virtual void BeginPlay() override;

	UFUNCTION()
	void SpawnNewEnemy();

	UFUNCTION()
	void ReturnEnemy(ABaseAICharacter* Enemy);

private:

	void InitSpawnEnemys();

	FVector CalculatingSpawnPoint();

	TQueue<ABaseAICharacter*> FreeEnemys;

	FDelegateHandle OnCharacterDeathHandle;

};
