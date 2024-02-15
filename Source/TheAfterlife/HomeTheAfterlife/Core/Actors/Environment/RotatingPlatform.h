// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlatformBase.h"
#include "Components/TimelineComponent.h"
#include "RotatingPlatform.generated.h"

/**
 * 
 */
UCLASS()
class THEAFTERLIFE_API ARotatingPlatform : public APlatformBase
{
	GENERATED_BODY()
	
public:
	virtual void Tick(float DeltaTime) override;

protected:

	virtual void BeginPlay() override;

	virtual void OnPlatformTriggerActivated(bool bIsActivated) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Platform movement")
	UCurveFloat* MovementCurve;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rotation Settings")
	float First = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rotation Settings")
	float Second = 90.0f;

private:

	bool bIsReached = false;

	FTimeline PlatformTimeline;

	UFUNCTION()
	void TickPlatformTimeline(float Value);

	//UFUNCTION()
	//void OnPlatfromEndReached();

};
