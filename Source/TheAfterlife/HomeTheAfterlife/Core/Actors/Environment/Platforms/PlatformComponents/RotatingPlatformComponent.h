// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/TimelineComponent.h"
#include "RotatingPlatformComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class THEAFTERLIFE_API URotatingPlatformComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	URotatingPlatformComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnPlatformTriggerActivated(bool bIsActivated);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Platform movement")
	UCurveFloat* MovementCurve;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rotation Settings")
	float First = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rotation Settings")
	float Second = 90.0f;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Platform movement")
	class APlatformTrigger* PlatformTrigger;

private:	
	TWeakObjectPtr<class APlatformBase> CachedPlatformBase;

	bool bIsReached = false;

	FTimeline PlatformTimeline;

	UFUNCTION()
	void TickPlatformTimeline(float Value);
};
