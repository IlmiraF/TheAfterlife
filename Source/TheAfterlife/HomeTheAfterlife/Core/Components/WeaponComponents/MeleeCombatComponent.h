// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MeleeCombatComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class THEAFTERLIFE_API UMeleeCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMeleeCombatComponent();

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
	//class ABaseCharacter* BaseCharacter;

	void MeleeAttackStart();
	void MeleeAttackFinish();


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character|Collisions")
	//class UBoxComponent* LeftHandCollision;
	//
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character|Collisions")
	//class UBoxComponent* RightHandCollision;


public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
