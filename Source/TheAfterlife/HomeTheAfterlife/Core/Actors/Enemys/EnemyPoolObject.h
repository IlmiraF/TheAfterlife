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


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn")
	int32 Cols = 3;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn")
	float DistanceBetweenRows = 100;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn")
	float DistanceBetweenCols = 100;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn")
	TArray<TSubclassOf<ABaseAICharacter>> InitEnemysArray;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "SpawnPoint", meta = (MakeEditWidget))
	FVector InitSpawnPoint;


protected:

	virtual void BeginPlay() override;

	UFUNCTION()
	void SwitchPosition(int32 Index);

private:

	void InitSpawnEnemys();

	TArray<ABaseAICharacter*> EnemysArray;

	int32 CurrentCols;
	int32 CurrentRows;
};
