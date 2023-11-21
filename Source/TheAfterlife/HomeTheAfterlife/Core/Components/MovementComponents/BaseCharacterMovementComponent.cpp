// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacterMovementComponent.h"
#include "../../Characters/BaseCharacter.h"
#include "Curves/CurveVector.h"

float UBaseCharacterMovementComponent::GetMaxSpeed() const
{
    float Result = Super::GetMaxSpeed();
    return Result;
}

void UBaseCharacterMovementComponent::StartMantle(const FMantlingMovementParameters& MantlingParameters)
{
    CurrentMantlingParameters = MantlingParameters;
    SetMovementMode(EMovementMode::MOVE_Custom, (uint8)ECustomMovementMode::CMOVE_Mantling);
}

void UBaseCharacterMovementComponent::EndMantle()
{
    GetBaseCharacterOwner()->bIsMantling = false;
    SetMovementMode(MOVE_Walking);
}

bool UBaseCharacterMovementComponent::IsMantling() const
{
    return UpdatedComponent && MovementMode == MOVE_Custom && CustomMovementMode == (uint8)ECustomMovementMode::CMOVE_Mantling;
}

void UBaseCharacterMovementComponent::OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PreviousMovementMode, PreviousCustomMode);

	if (MovementMode == MOVE_Custom)
	{
		switch (CustomMovementMode)
		{
		case (uint8)ECustomMovementMode::CMOVE_Mantling:
		{
			GetWorld()->GetTimerManager().SetTimer(MantlingTimer, this, &UBaseCharacterMovementComponent::EndMantle, CurrentMantlingParameters.Duration, false);
			break;
		}
		default:
			break;
		}
	}
}

void UBaseCharacterMovementComponent::PhysCustom(float DeltaTime, int32 Iterations)
{
	switch (CustomMovementMode)
	{
		case (uint8)ECustomMovementMode::CMOVE_Mantling:
		{
			PhysMantling(DeltaTime, Iterations);
			break;
		}
		default:
			break;
	}

	Super::PhysCustom(DeltaTime, Iterations);
}

void UBaseCharacterMovementComponent::PhysMantling(float DeltaTime, int32 Iterations)
{
	float ElapsedTime = GetWorld()->GetTimerManager().GetTimerElapsed(MantlingTimer) + CurrentMantlingParameters.StartTime;

	FVector MantlingCurveValue = CurrentMantlingParameters.MantlingCurve->GetVectorValue(ElapsedTime);

	float PositionAlpha = MantlingCurveValue.X;
	float XYCorrectionAlpha = MantlingCurveValue.Y;
	float ZCorrectionAlpha = MantlingCurveValue.Z;

	FVector CorrectedInitialLocation = FMath::Lerp(CurrentMantlingParameters.InitialLocation, CurrentMantlingParameters.InitialAnimationLocation, XYCorrectionAlpha);
	CorrectedInitialLocation.Z = FMath::Lerp(CurrentMantlingParameters.InitialLocation.Z, CurrentMantlingParameters.InitialAnimationLocation.Z, ZCorrectionAlpha);


	FVector NewLocation = FMath::Lerp(CorrectedInitialLocation, CurrentMantlingParameters.TargetLocation, PositionAlpha);
	FRotator NewRotation = FMath::Lerp(CurrentMantlingParameters.InitialRotation, CurrentMantlingParameters.TargetRotation, PositionAlpha);

	FVector Delta = NewLocation - GetActorLocation();
	Velocity = Delta / DeltaTime;

	FHitResult Hit;
	SafeMoveUpdatedComponent(Delta, NewRotation, false, Hit);
}

ABaseCharacter* UBaseCharacterMovementComponent::GetBaseCharacterOwner() const
{
	return StaticCast<ABaseCharacter*>(CharacterOwner);
}
