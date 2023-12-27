// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlatformTrigger.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTriggerActivated, bool, bIsActivated);

class UBoxComponent;
class APawn;
UCLASS()
class THEAFTERLIFE_API APlatformTrigger : public AActor
{
	GENERATED_BODY()
	
public:
	APlatformTrigger();

	UPROPERTY(BlueprintAssignable)
	FOnTriggerActivated OnTriggerActivated;

	//void SetIsActivated(bool bIsActivated_In);

	//UFUNCTION(NetMulticast, Unreliable)
	//	void Multicast_SetIsActivated(bool bIsActivated_In);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UBoxComponent* TriggerBox;

	bool bIsActivated = false;

private:

	void SetIsActivated(bool bIsActivated_In);

	TArray<APawn*> OverlappedPawns;

	UFUNCTION()
	void OnTriggerOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnTriggerOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
