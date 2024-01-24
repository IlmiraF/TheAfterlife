// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../InteractiveActor.h"
#include "RunWall.generated.h"

/**
 * 
 */
class UAnimMontage;
class UBoxComponent;
class UStaticMeshComponent;
UCLASS()
class THEAFTERLIFE_API ARunWall : public AInteractiveActor
{
	GENERATED_BODY()
	
public:
	ARunWall();

	virtual void OnConstruction(const FTransform& Transform) override;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wall Parameters")
	float WallHeight = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wall Parameters")
	float WallWidth = 50.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* WallMeshComponent;

	UBoxComponent* GetInteractionBox() const;
};
