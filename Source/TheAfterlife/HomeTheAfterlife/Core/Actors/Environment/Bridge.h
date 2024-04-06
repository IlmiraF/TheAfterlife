// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Door.h"
#include "Bridge.generated.h"


UCLASS()
class THEAFTERLIFE_API ABridge : public AActor
{
	GENERATED_BODY()
	
public:	
	ABridge();

protected:

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* BridgeMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Subscriptions")
	ADoor* Door;

private:

	void DisablingBridge();


	UPROPERTY(EditDefaultsOnly, Category = "Components")
	UAudioComponent* BridgeAudioComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	USoundBase* SoundDestruction;
};
