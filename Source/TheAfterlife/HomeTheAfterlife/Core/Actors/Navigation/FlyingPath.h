// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SplineComponent.h"
#include "FlyingPath.generated.h"


UCLASS()
class THEAFTERLIFE_API AFlyingPath : public AActor
{
	GENERATED_BODY()
	
public:	

	AFlyingPath();

	USplineComponent* GetFlyingSpline();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spline", meta = (MakeEditWidget))
	USplineComponent* SplineComponent;
};
