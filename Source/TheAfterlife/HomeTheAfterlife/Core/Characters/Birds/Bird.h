// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/SplineComponent.h"
#include "..\..\Actors\Interfaces\ISpeak.h"
#include "Bird.generated.h"

UCLASS()
class THEAFTERLIFE_API ABird : public APawn, public ISpeak
{
	GENERATED_BODY()

public:
	ABird();

	virtual void Tick(float DeltaTime) override;

	void SetNewPoint(int32 Index);

	virtual void Speak(USoundBase* SoundBase) override;

protected:

	virtual void BeginPlay() override;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spline")
	USplineComponent* SplineComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	class USkeletalMeshComponent* BirdMesh;

private:

	UAudioComponent* BirdAudioComponent;

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

	int32 CurrentIndex = 0;

	void Fly(float DeltaTime);

	float DistanceAlongSpline;

	TArray<float> StopDistances;

	void InitializeSpline();

};