// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacterMovementComponent.h"
#include "../../Characters/BaseCharacter.h"
#include "Curves/CurveVector.h"
#include "../../Actors/Interactive/Environment/Ladder.h"
#include "../../Actors/Interactive/Environment/Zipline.h"
#include "Components/CapsuleComponent.h"

float UBaseCharacterMovementComponent::GetMaxSpeed() const
{
    float Result = Super::GetMaxSpeed();
	if (IsOnLadder())
	{
		Result = ClimbingOnLadderMaxSpeed;
	}
	if (IsOnZipline())
	{
		Result = MaxSpeedOnZipline;
	}
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

void UBaseCharacterMovementComponent::AttachToLadder(const ALadder* Ladder)
{
	CurrentLadder = Ladder;

	FRotator TargetOrientationRotation = CurrentLadder->GetActorForwardVector().ToOrientationRotator();
	TargetOrientationRotation.Yaw += 180.0f;

	FVector LadderUpVector = CurrentLadder->GetActorUpVector();
	FVector LadderForwardVector = CurrentLadder->GetActorForwardVector();
	float Projection = GetActorToCurrentLadderProjection(GetActorLocation());

	FVector NewCharacterLocation = CurrentLadder->GetActorLocation() + Projection * LadderUpVector + LadderToCharacterOffset * LadderForwardVector;
	if (CurrentLadder->GetIsOnTop())
	{
		NewCharacterLocation = CurrentLadder->GetAttachFromTopAnimMontageStartingLocation();
	}

	GetOwner()->SetActorLocation(NewCharacterLocation);

	GetOwner()->SetActorRotation(TargetOrientationRotation);

	SetMovementMode(MOVE_Custom, (uint8)ECustomMovementMode::CMOVE_Ladder);
}

void UBaseCharacterMovementComponent::DetachFromLadder(EDetachFromLadderMethod DetachFromLadderMethod)
{
	switch (DetachFromLadderMethod)
	{
		case EDetachFromLadderMethod::ReachingTheTop:
		{
			GetBaseCharacterOwner()->Mantle(true);
			break;
		}
		case EDetachFromLadderMethod::ReachingTheBottom:
		{
			SetMovementMode(MOVE_Walking);
			break;
		}
		case EDetachFromLadderMethod::JumpOff:
		{
			FVector JumpDirection = CurrentLadder->GetActorForwardVector();
			SetMovementMode(MOVE_Falling);
			FVector JumpVelocity = JumpDirection * JumpOffFromLadderSpeed;
			ForceTargetRotation = JumpDirection.ToOrientationRotator();
			bForceRotation = true;
			Launch(JumpVelocity);
			break;
		}
		case EDetachFromLadderMethod::Fall:
		default:
		{
			SetMovementMode(MOVE_Falling);
			break;
		}
	}
}

bool UBaseCharacterMovementComponent::IsOnLadder() const
{
	return UpdatedComponent && MovementMode == MOVE_Custom && CustomMovementMode == (uint8)ECustomMovementMode::CMOVE_Ladder;
}

const ALadder* UBaseCharacterMovementComponent::GetCurrentLadder()
{
	return CurrentLadder;
}

float UBaseCharacterMovementComponent::GetLadderSpeedRatio() const
{
	checkf(IsValid(CurrentLadder), TEXT("float UGCBaseCharacterMovementComponent::GetLadderSpeedRatio() cannot be invoked when current ladder is null"))

	FVector LadderUpVector = CurrentLadder->GetActorUpVector();
	return FVector::DotProduct(LadderUpVector, Velocity) / ClimbingOnLadderMaxSpeed;
}

float UBaseCharacterMovementComponent::GetActorToCurrentLadderProjection(const FVector& Location) const
{
	checkf(IsValid(CurrentLadder), TEXT("float UGCBaseCharacterMovementComponent::GetCharacterToCurrentLadderProjection() cannot be invoked when current ladder is null"))

	FVector LadderUpVector = CurrentLadder->GetActorUpVector();
	FVector LadderToCharacterDistance = Location - CurrentLadder->GetActorLocation();
	float ActorToLadderProjection = FVector::DotProduct(LadderUpVector, LadderToCharacterDistance);

	return ActorToLadderProjection;
}

void UBaseCharacterMovementComponent::AttachToZipline(const AZipline* Zipline)
{
	CurrentZipline = Zipline;
	FRotator InteractiveCapsuleRotation = CurrentZipline->GetZiplineInteractiveCapsule()->GetRelativeRotation();
	FRotator TargetOrientationRotation = FRotator(0.0f, InteractiveCapsuleRotation.Yaw, InteractiveCapsuleRotation.Roll);
	float Projection = GetActorToCurrentZiplineProjection(GetActorLocation());
	FVector ZiplineForwardVector = CurrentZipline->GetZiplineInteractiveCapsule()->GetForwardVector();
	FVector ZiplineUpVector = CurrentZipline->GetZiplineInteractiveCapsule()->GetUpVector();

	FVector NewCharacterLocation = CurrentZipline->GetZiplineInteractiveCapsule()->GetComponentLocation() + Projection * ZiplineUpVector - (CurrentZipline->GetZiplineInteractiveCapsule()->GetUnscaledCapsuleRadius() + ZiplineToCharacterOffset) * ZiplineForwardVector;

	GetOwner()->SetActorLocation(NewCharacterLocation);
	GetOwner()->SetActorRotation(TargetOrientationRotation);

	SetMovementMode(MOVE_Custom, (uint8)ECustomMovementMode::CMOVE_Zipline);
}

void UBaseCharacterMovementComponent::DetachFromZipline()
{
	SetMovementMode(MOVE_Falling);
}

float UBaseCharacterMovementComponent::GetActorToCurrentZiplineProjection(const FVector& Location) const
{
	checkf(IsValid(CurrentZipline), TEXT("UGCBaseCharacterMovementComponent::GetCharacterToCurrentZiplineProjection() cannot be invoked when current zipline is null"));
	FVector ZiplineUpVector = CurrentZipline->GetZiplineInteractiveCapsule()->GetUpVector();
	FVector ZiplineToCharacterDistance = Location - CurrentZipline->GetZiplineInteractiveCapsule()->GetComponentLocation();
	return FVector::DotProduct(ZiplineUpVector, ZiplineToCharacterDistance);
}

bool UBaseCharacterMovementComponent::IsOnZipline() const
{
	return UpdatedComponent && MovementMode == MOVE_Custom && CustomMovementMode == (uint8)ECustomMovementMode::CMOVE_Zipline;
}

void UBaseCharacterMovementComponent::OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PreviousMovementMode, PreviousCustomMode);

	if (PreviousMovementMode == MOVE_Custom && PreviousCustomMode == (uint8)ECustomMovementMode::CMOVE_Ladder)
	{
		CurrentLadder = nullptr;
	}

	if (PreviousMovementMode == MOVE_Custom && PreviousCustomMode == (uint8)ECustomMovementMode::CMOVE_Zipline)
	{
		CurrentZipline = nullptr;
	}

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
		case (uint8)ECustomMovementMode::CMOVE_Ladder:
		{
			PhysLadder(DeltaTime, Iterations);
			break;
		}
		case (uint8)ECustomMovementMode::CMOVE_Zipline:
		{
			PhysZipline(DeltaTime, Iterations);
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

void UBaseCharacterMovementComponent::PhysLadder(float DeltaTime, int32 Iterations)
{
	CalcVelocity(DeltaTime, 1.0f, false, ClimbingOnLadderBrakingDecelaration);
	FVector Delta = Velocity * DeltaTime;

	if (HasAnimRootMotion())
	{
		FHitResult Hit;
		SafeMoveUpdatedComponent(Delta, GetOwner()->GetActorRotation(), false, Hit);
		return;
	}

	FVector NewPos = GetActorLocation() + Delta;
	float NewPosProjection = GetActorToCurrentLadderProjection(NewPos);

	if (NewPosProjection < MinLadderBottomOffset)
	{
		DetachFromLadder(EDetachFromLadderMethod::ReachingTheBottom);
		return;
	}
	else if (NewPosProjection > (CurrentLadder->GetLadderHeight() - MaxLadderTopOffset))
	{
		DetachFromLadder(EDetachFromLadderMethod::ReachingTheTop);
		return;
	}

	FHitResult Hit;
	SafeMoveUpdatedComponent(Delta, GetOwner()->GetActorRotation(), true, Hit);
}

void UBaseCharacterMovementComponent::PhysZipline(float DeltaTime, int32 Iterations)
{
	FVector ZiplineDirection = (CurrentZipline->GetTopPointOfSecondPillar() - CurrentZipline->GetTopPointOfFirstPillar()).GetSafeNormal();
	FVector Delta = MaxSpeedOnZipline * ZiplineDirection * DeltaTime;

	FHitResult Hit;
	SafeMoveUpdatedComponent(Delta, GetOwner()->GetActorRotation(), true, Hit);
	if (Hit.IsValidBlockingHit())
	{
		SetMovementMode(MOVE_Walking);
	}
}

ABaseCharacter* UBaseCharacterMovementComponent::GetBaseCharacterOwner() const
{
	return StaticCast<ABaseCharacter*>(CharacterOwner);
}
