// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TutorialCollider.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnReachedTarget);

class UBoxComponent;
UCLASS()
class THEAFTERLIFE_API ATutorialCollider : public AActor
{
	GENERATED_BODY()

public:

	ATutorialCollider();

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* Collider;

	UPROPERTY(EditAnywhere)
	FString TutorialText;

	FOnReachedTarget OnReachedTargetEvent;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:

	void OnReachedTarget();
};