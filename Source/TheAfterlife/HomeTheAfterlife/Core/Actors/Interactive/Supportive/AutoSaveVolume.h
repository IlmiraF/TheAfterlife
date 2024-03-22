// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AutoSaveVolume.generated.h"

UCLASS(Blueprintable)
class THEAFTERLIFE_API AAutoSaveVolume : public AActor
{
	GENERATED_BODY()

public:

	AAutoSaveVolume();

	//virtual void OnConstruction(const FTransform& Transform) override;

	virtual void BeginPlay() override;
	
protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction")
	class UPrimitiveComponent* InteractionVolume;

	UPROPERTY(EditInstanceOnly)
	bool bIsStealingHealth = false;

	UPROPERTY(EditInstanceOnly, meta=(UIMin = 0.0f, UIMax = 1.0f))
	float HealthStealingRatio = 0.25f;

	UFUNCTION()
	virtual void OnInteractionVolumeOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	bool IsOverlappingCharacterCapsule(AActor* OtherActor, UPrimitiveComponent* OtherComp);

private:

	bool bIsSaved = false;
};
