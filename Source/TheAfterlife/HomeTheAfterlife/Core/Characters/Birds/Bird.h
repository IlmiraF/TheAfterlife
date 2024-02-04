// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Bird.generated.h"

UCLASS()
class THEAFTERLIFE_API ABird : public APawn
{
	GENERATED_BODY()

public:
	ABird();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void SetNewPoint(int32 Index);

private:

	UPROPERTY(EditAnywhere)
	TArray<FVector> RouteArray;

	UPROPERTY(EditDefaultsOnly)
	float Speed = 30.0f;

	UPROPERTY(EditDefaultsOnly)
	float SinusoidHeight = 3.0f;

	UPROPERTY(EditDefaultsOnly)
	float SinusoidFrequency = 1.0f;

	UPROPERTY(EditDefaultsOnly)
	float DeltaPosition = 10.0f;

	int32 CurrnetIndex = 0;

	void Fly(float DeltaTime);

	float GetSinusoidOffset(float DeltaTime, float Height, float Frequency);

	float CurrentTime;
};