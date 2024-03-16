// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LightSourse.h"
#include "Components/BoxComponent.h"
#include "LightSourceController.generated.h"


UCLASS()
class THEAFTERLIFE_API ALightSourceController : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALightSourceController();

protected:


	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "LightSourses", meta = (MakeEditWidget))
	TArray<ALightSourse*> LightSourses;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	UBoxComponent* TriggerComponent;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


private:

	void TurnOnLight();
};
