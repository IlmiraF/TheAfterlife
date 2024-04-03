// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Interactive/Interactive.h"
#include "LightSourceController.generated.h"

class UBoxComponent;
class ALightSourse;
class ABaseCharacter;
UCLASS()
class THEAFTERLIFE_API ALightSourceController : public AActor, public IInteractable
{
	GENERATED_BODY()
	
public:	

	ALightSourceController();

	virtual void Interact(ABaseCharacter* Character) override;
	virtual bool IsForce() override;

protected:

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "LightSourses", meta = (MakeEditWidget))
	TArray<ALightSourse*> LightSourses;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	UBoxComponent* TriggerComponent;

private:

	UPROPERTY(EditAnywhere)
	bool bIsForceIntecation = true;

	void TurnOnLight();
};
