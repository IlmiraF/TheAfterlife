// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/TimelineComponent.h"
#include "MovingPlatformComponent.generated.h"

class UStaticMeshComponent;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class THEAFTERLIFE_API UMovingPlatformComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UMovingPlatformComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void MovePlatform();

protected:

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Platform movement")
	UCurveFloat* MovementCurve;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Platform movement", meta = (MakeEditWidget))
	FVector FirstEndPlatformLocation;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Platform movement", meta = (MakeEditWidget))
	FVector SecondEndPlatformLocation;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Platform movement", meta = (MakeEditWidget))
	float ReturnTime = 0.0;

	UFUNCTION()
	void OnPlatformTriggerActivated(bool bIsActivated);

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Platform movement")
	class APlatformTrigger* PlatformTrigger;

private:	

	TWeakObjectPtr<class APlatformBase> CachedPlatformBase;

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
