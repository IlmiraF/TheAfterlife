// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AISense.h"
#include "BaseAIController.generated.h"

/**
 * 
 */
UCLASS()
class THEAFTERLIFE_API ABaseAIController : public AAIController
{
	GENERATED_BODY()

public:

	ABaseAIController();

protected:

	AActor* GetClosestSesnedActor(TSubclassOf<UAISense> SenseClass) const;
	
};
