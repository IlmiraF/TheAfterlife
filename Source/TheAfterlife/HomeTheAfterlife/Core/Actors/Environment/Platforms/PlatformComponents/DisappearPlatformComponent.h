// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/TimelineComponent.h"
#include "DisappearPlatformComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class THEAFTERLIFE_API UDisappearPlatformComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UDisappearPlatformComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnTriggerOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void OnTriggerOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Platform shine")
	UCurveFloat* ShineCurve;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Platform shine")
	float ShineDelay = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Platform shine")
	float ShineIntensity = 10.0f;

	UMaterialInstanceDynamic* BaseMaterial;

private:

	TWeakObjectPtr<class APlatformBase> CachedPlatformBase;

	FTimerHandle TimerHandle;
	FTimeline PlatformTimeline;

	UFUNCTION()
	void TickPlatformTimeline(float Value);

	void Disappear();
};
