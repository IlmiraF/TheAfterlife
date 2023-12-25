// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../InteractiveActor.h"
#include "Beam.generated.h"

/**
 * 
 */

class UAnimMontage;
class UBoxComponent;
class UStaticMeshComponent;
UCLASS()
class THEAFTERLIFE_API ABeam : public AInteractiveActor
{
	GENERATED_BODY()

public:
	ABeam();

	virtual void OnConstruction(const FTransform& Transform) override;

	float GetBeamWidth() const;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Beam Parameters")
	float BeamHeight = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Beam Parameters")
	float BeamWidth = 200.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* BeamMeshComponent;

	UBoxComponent* GetBeamInteractionBox() const;

	virtual void OnInteractionVolumeOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void OnInteractionVolumeOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;
	
};
