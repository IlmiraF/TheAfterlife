// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Medkit.generated.h"

class ABaseCharacter;
class UStaticMeshComponent;
UCLASS()
class THEAFTERLIFE_API AMedkit : public AActor
{
	GENERATED_BODY()

public:

	AMedkit();

	virtual void BeginPlay() override;

protected:
	void Consume(ABaseCharacter* ConsumeTarget);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* MedkitMeshComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Medkit")
	float Health = 25.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction")
	class UPrimitiveComponent* InteractionVolume;

	UFUNCTION()
	virtual void OnInteractionVolumeOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
