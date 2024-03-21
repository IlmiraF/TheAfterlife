// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/SplineComponent.h"
#include "Bird.generated.h"

UCLASS()
class THEAFTERLIFE_API ABird : public APawn
{
	GENERATED_BODY()

public:
	ABird();

	virtual void Tick(float DeltaTime) override;

	void SetNewPoint(int32 Index);

protected:

	virtual void BeginPlay() override;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spline")
	USplineComponent* SplineComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	class USkeletalMeshComponent* BirdMesh;

private:

	UPROPERTY(EditAnywhere)
	TArray<FVector> RouteArray;

	UPROPERTY(EditAnywhere)
	float Speed = 100.0f;

	UPROPERTY(EditDefaultsOnly)
	float SinusoidHeight = 3.0f;

	UPROPERTY(EditDefaultsOnly)
	float SinusoidFrequency = 1.0f;

	UPROPERTY(EditDefaultsOnly)
	float DeltaPosition = 10.0f;

	int32 CurrnetIndex = 0;

	void Fly(float DeltaTime);

	float GetSinusoidOffset(float DeltaTime, float Height, float Frequency);

	float CurrentIndex;

	float CurrentTime;

	float DistanceAlongSpline;

	float StopDistance;

	float SplineLength;

	float DistanceToTarget;

	TArray<float> StopDistances;

	void InitializeSpline();

};