// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TheAfterlife/HomeTheAfterlife/Core/Actors/Navigation/PatrollingPath.h"
#include "AIPatrollingComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class THEAFTERLIFE_API UAIPatrollingComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	bool CanPatrol() const;

	FVector SelectClosestWayPoint();
	FVector SelectNextWayPoint();

protected:

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Path")
	APatrollingPath* PatrollingPath;

private:

	int32 CurretWayPointIndex = -1;	
};
