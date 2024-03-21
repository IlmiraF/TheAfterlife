// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlatformBase.h"
#include "Components/TimelineComponent.h"
#include "DisappearPlatform.generated.h"

/**
 *
 */
UCLASS()
class THEAFTERLIFE_API ADisappearPlatform : public APlatformBase
{
	GENERATED_BODY()

public:
	virtual void Tick(float DeltaTime) override;

protected:

	virtual void BeginPlay() override;

	virtual void OnTriggerOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void OnTriggerOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Platform shine")
	UCurveFloat* ShineCurve;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Platform shine")
	float ShineDelay = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Platform shine")
	float ShineIntensity = 10.0f;

	UMaterialInstanceDynamic* BaseMaterial;

private:

	FTimerHandle TimerHandle;
	FTimeline PlatformTimeline;

	UFUNCTION()
	void TickPlatformTimeline(float Value);

	void Disappear();
};