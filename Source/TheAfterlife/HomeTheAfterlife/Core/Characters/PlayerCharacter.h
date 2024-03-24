// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "..\..\Core\Actors\Interfaces\ISpeak.h"
#include "PlayerCharacter.generated.h"

/**
 * 
 */
UCLASS()
class THEAFTERLIFE_API APlayerCharacter : public ABaseCharacter, public ISpeak
{
	GENERATED_BODY()

public:
	APlayerCharacter(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;

	virtual void MoveForward(float value) override;
	virtual void MoveRight(float value) override;
	virtual void Turn(float value) override;
	virtual void LookUp(float value) override;
	virtual void Jump() override;
	virtual void Landed(const FHitResult& Hit) override;

	virtual void ClimbMoveForward(float value) override;
	virtual void ClimbMoveRight(float value) override;
	virtual void ClimbHop() override;

	virtual void OnBeamMoveForward(float value) override;
	virtual void OnBeamMoveRight(float value) override;

	virtual void Speak(USoundBase* SoundBase) override;

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character|Camera")
	class UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character|Camera")
	class USpringArmComponent* SpringArmComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character|Jump")
	class UAnimMontage* DoubleJumpMontage;

	virtual void OnStartAimingIternal() override;
	virtual void OnStopAimingIternal() override;
};
