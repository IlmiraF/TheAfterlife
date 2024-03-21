// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlatformBase.generated.h"

class UBoxComponent;
class UStaticMeshComponent;
class APlatformTrigger;
UCLASS()
class THEAFTERLIFE_API APlatformBase : public AActor
{
	GENERATED_BODY()

public:
	APlatformBase();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Platform movement")
	APlatformTrigger* PlatformTrigger;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Platform movement")
	UStaticMeshComponent* PlatformMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UBoxComponent* TriggerBox;

	UFUNCTION()
	virtual void OnTriggerOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnTriggerOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	virtual void OnPlatformTriggerActivated(bool bIsActivated);

};