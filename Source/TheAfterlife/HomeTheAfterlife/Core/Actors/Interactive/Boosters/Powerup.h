// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Interactive.h"
#include "Powerup.generated.h"

/**
 * 
 */
class UBoxComponent;
UCLASS()
class THEAFTERLIFE_API APowerup : public AActor, public IInteractable
{
	GENERATED_BODY()

public:

	APowerup();

	virtual void Interact(ABaseCharacter* ConsumeTarget) override;
	virtual bool IsForce() override;

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Medkit")
	UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Medkit")
	UBoxComponent* InteractionVolume;

	UPROPERTY(EditDefaultsOnly, Category="Medkit")
	float HealthToAdd = 25.0f;
	
};
