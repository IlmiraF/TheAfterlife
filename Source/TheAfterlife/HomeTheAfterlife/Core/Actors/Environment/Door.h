// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Interactive/Interactive.h"
#include "Door.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnDoorsOpening);

class UBoxComponent;
class ABaseCharacter;
UCLASS()
class THEAFTERLIFE_API ADoor : public AActor, public IInteractable
{
	GENERATED_BODY()
	
public:	

	ADoor();

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	UBoxComponent* TriggerComponent;

	virtual void Interact(ABaseCharacter* Character) override;
	virtual bool IsForce() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UStaticMeshComponent* LeftDoor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UStaticMeshComponent* RightDoor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Doors")
	float DoorOpenSpeed = 20.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Doors")
	float OpenAngle = 120.0f;

	FOnDoorsOpening OnDoorsOpening;

private:

	void OpenDoors(float DeltaTime);

	UPROPERTY(EditAnywhere)
	bool bIsForce = true;

	bool bIsDoorOpened = false;

	bool bStartOpen = false;

};
