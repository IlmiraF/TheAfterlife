// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "PlayerCharacter.generated.h"

/**
 * 
 */
UCLASS()
class THEAFTERLIFE_API APlayerCharacter : public ABaseCharacter
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
	virtual void Falling() override;
	virtual void NotifyJumpApex() override;

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character|Camera")
	class UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character|Camera")
	class USpringArmComponent* SpringArmComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character|Jump")
	class UAnimMontage* DoubleJumpMontage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character|Components")
	class UCharacterAttributeComponent* CharacterAttributeComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character | Animations")
	class UAnimMontage* OnDeathAnimMontage;
	// Damage depending from fall height (in meters)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character | Attributes")
	class UCurveFloat* FallDamageCurve;

	virtual void OnDeath();

private:
	int32 JumpCount = 0;
	
	void EnableRagdoll();

	FVector CurrentFallApex;
};
