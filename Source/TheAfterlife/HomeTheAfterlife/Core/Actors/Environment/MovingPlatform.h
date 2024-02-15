// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlatformBase.h"
#include "Components/TimelineComponent.h"
#include "MovingPlatform.generated.h"


UENUM(BlueprintType)
enum class EPlatformBehavior : uint8
{
	ForwardBackward = 0,
	LeftRight
};

/**
 * 
 */
class UStaticMeshComponent;
UCLASS()
class THEAFTERLIFE_API AMovingPlatform : public APlatformBase
{
	GENERATED_BODY()

public:
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void MovePlatform();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Platform movement")
	UCurveFloat* MovementCurve;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Platform movement", meta = (MakeEditWidget))
	FVector FirstEndPlatformLocation;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Platform movement", meta = (MakeEditWidget))
	FVector SecondEndPlatformLocation;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Platform movement", meta = (MakeEditWidget))
	float ReturnTime = 0.0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Platform movement")
	EPlatformBehavior PlatformBehavior;

	virtual void BeginPlay() override;

	virtual void OnPlatformTriggerActivated(bool bIsActivated) override;

private:

	UFUNCTION()
	void TickPlatformTimeline(float Value);

	UFUNCTION()
	void OnPlatfromEndReached();

	FTimeline PlatformTimeline;

	FVector StartLocation = FVector::ZeroVector;
	FVector EndLocation = FVector::ZeroVector;
	bool bIsMovingForward = false;

	FTimerHandle ReturnTimer;

	bool bIsForwardBackward = false;

	bool bIsFirstTime = true;
};
