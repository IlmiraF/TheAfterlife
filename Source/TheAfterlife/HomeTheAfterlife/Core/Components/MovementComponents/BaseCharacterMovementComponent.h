// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BaseCharacterMovementComponent.generated.h"

struct FMantlingMovementParameters
{
	FVector InitialLocation = FVector::ZeroVector;
	FRotator InitialRotation = FRotator::ZeroRotator;

	FVector TargetLocation = FVector::ZeroVector;
	FRotator TargetRotation = FRotator::ZeroRotator;

	FVector InitialAnimationLocation = FVector::ZeroVector;

	float Duration = 1.0f;
	float StartTime = 0.0f;

	UCurveVector* MantlingCurve;
};

UENUM(BlueprintType)
enum class ECustomMovementMode : uint8
{
	CMOVE_None = 0 UMETA(DisplayName = "None"),
	CMOVE_Mantling UMETA(DisplayName = "Mantling"),
	CMOVE_Max UMETA(Hidden)
};

/**
 * 
 */

UCLASS()
class THEAFTERLIFE_API UBaseCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:
	virtual float GetMaxSpeed() const override;

	void StartMantle(const FMantlingMovementParameters& MantlingParameters);
	void EndMantle();
	bool IsMantling() const;
	
protected:

	virtual void OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode) override;
	
	virtual void PhysCustom(float DeltaTime, int32 Iterations) override;

	void PhysMantling(float DeltaTime, int32 Iterations);

	class ABaseCharacter* GetBaseCharacterOwner() const;

private:
	FMantlingMovementParameters CurrentMantlingParameters;

	FTimerHandle MantlingTimer;

};
