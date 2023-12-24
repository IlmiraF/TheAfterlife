// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacterMovementComponent.h"
#include "../../Characters/BaseCharacter.h"
#include "Curves/CurveVector.h"
#include "../../Actors/Interactive/Environment/Ladder.h"
#include "../../Actors/Interactive/Environment/Zipline.h"
#include "Components/CapsuleComponent.h"
#include "../../../../TheAfterlifeTypes.h"
#include "Kismet/KismetMathLibrary.h"
#include "../../Utils/TheAfterlife_TraceUtils.h"
#include "MotionWarpingComponent.h"

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
	if (IsWallRunning())
	{
		Result = MaxWallRunSpeed;
	}
	if (IsClimbing())
	{
		Result = ClimbMaxSpeed;
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

void UBaseCharacterMovementComponent::PhysicsRotation(float DeltaTime)
{
	if (bForceRotation)
	{
		FRotator CurrentRotation = UpdatedComponent->GetComponentRotation();
		CurrentRotation.DiagnosticCheckNaN(TEXT("UGCBaseCharacterMovementComponent::PhysicsRotation(): CurrentRotation"));

		FRotator DeltaRot = GetDeltaRotation(DeltaTime);
		DeltaRot.DiagnosticCheckNaN(TEXT("UGCBaseCharacterMovementComponent::PhysicsRotation(): GetDeltaRotation"));

		// Accumulate a desired new rotation.
		const float AngleTolerance = 1e-3f;

		if (!CurrentRotation.Equals(ForceTargetRotation, AngleTolerance))
		{
			FRotator DesiredRotation = ForceTargetRotation;
			// PITCH
			if (!FMath::IsNearlyEqual(CurrentRotation.Pitch, DesiredRotation.Pitch, AngleTolerance))
			{
				DesiredRotation.Pitch = FMath::FixedTurn(CurrentRotation.Pitch, DesiredRotation.Pitch, DeltaRot.Pitch);
			}

			// YAW
			if (!FMath::IsNearlyEqual(CurrentRotation.Yaw, DesiredRotation.Yaw, AngleTolerance))
			{
				DesiredRotation.Yaw = FMath::FixedTurn(CurrentRotation.Yaw, DesiredRotation.Yaw, DeltaRot.Yaw);
			}

			// ROLL
			if (!FMath::IsNearlyEqual(CurrentRotation.Roll, DesiredRotation.Roll, AngleTolerance))
			{
				DesiredRotation.Roll = FMath::FixedTurn(CurrentRotation.Roll, DesiredRotation.Roll, DeltaRot.Roll);
			}

			// Set the new rotation.
			DesiredRotation.DiagnosticCheckNaN(TEXT("CharacterMovementComponent::PhysicsRotation(): DesiredRotation"));
			MoveUpdatedComponent(FVector::ZeroVector, DesiredRotation, /*bSweep*/ false);
		}
		else
		{
			ForceTargetRotation = FRotator::ZeroRotator;
			bForceRotation = false;
		}
		return;
	}

	if (IsOnLadder())
	{
		return;
	}
	Super::PhysicsRotation(DeltaTime);
}

bool UBaseCharacterMovementComponent::IsWallRunning() const
{
	return UpdatedComponent && MovementMode == MOVE_Custom && CustomMovementMode == (uint8)ECustomMovementMode::CMOVE_WallRun;
}

bool UBaseCharacterMovementComponent::IsClimbing() const
{
	return UpdatedComponent && MovementMode == MOVE_Custom && CustomMovementMode == (uint8)ECustomMovementMode::CMOVE_Parkour;
}

FVector UBaseCharacterMovementComponent::GetUnrotatedClimbVelocity() const
{
	return UKismetMathLibrary::Quat_UnrotateVector(UpdatedComponent->GetComponentQuat(), Velocity);
}

void UBaseCharacterMovementComponent::ToggleClimbing(bool bAttemptClimbing)
{
	if (bAttemptClimbing)
	{
		if (CanStartClimbing())
		{
			SetJumpAllowed(!bAttemptClimbing);
			PlayClimbMontage(IdleToClimbMontage);
		}
			/*else if (CanClimbDownLedge())
			{
				PlayClimbMontage(ClimbDownLedgeMontage);
			}*/
	}

	if (!bAttemptClimbing)
	{
		StopClimbing();
		SetJumpAllowed(!bAttemptClimbing);
	}
}

void UBaseCharacterMovementComponent::RequestHopping()
{
	const FVector UnrotatedLastInputVector =
		UKismetMathLibrary::Quat_UnrotateVector(UpdatedComponent->GetComponentQuat(), GetLastInputVector());

	const float DotResult =
		FVector::DotProduct(UnrotatedLastInputVector.GetSafeNormal(), FVector::RightVector);

	if (DotResult >= 0.9f)
	{
		HandleHopRight();
	}
	else if (DotResult <= -0.9f)
	{
		HandleHopLeft();
	}
}

bool UBaseCharacterMovementComponent::HasClimbing() const
{
	return bHasClimbing;
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

	if (PreviousMovementMode == MOVE_Custom && PreviousCustomMode == (uint8)ECustomMovementMode::CMOVE_Parkour)
	{
		bOrientRotationToMovement = true;
		CharacterOwner->GetCapsuleComponent()->SetCapsuleHalfHeight(96.f);

		// Reset pitch and roll
		const FRotator DirtyRotation = UpdatedComponent->GetComponentRotation();
		const FRotator CleanStandRotation = FRotator(0.f, DirtyRotation.Yaw, 0.f);
		UpdatedComponent->SetRelativeRotation(CleanStandRotation);

		bHasClimbing = true;

		StopMovementImmediately();
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
		case (uint8)ECustomMovementMode::CMOVE_WallRun:
		{
			FHitResult WallHit;
			Safe_bWallRunIsRight = IsWallOnSideTrace(WallHit, true);
			break;
		}
		case (uint8)ECustomMovementMode::CMOVE_Parkour:
		{
			bHasClimbing = false;
			bOrientRotationToMovement = false;
			CharacterOwner->GetCapsuleComponent()->SetCapsuleHalfHeight(48.f);
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
		case (uint8)ECustomMovementMode::CMOVE_WallRun:
		{
			PhysWallRun(DeltaTime, Iterations);
			break;
		}
		case (uint8)ECustomMovementMode::CMOVE_Parkour:
		{
			PhysClimb(DeltaTime, Iterations);
			break;
		}
		default:
			break;
	}

	Super::PhysCustom(DeltaTime, Iterations);
}

void UBaseCharacterMovementComponent::BeginPlay()
{
	Super::BeginPlay();
	UAnimInstance* AnimInstance = CharacterOwner->GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		AnimInstance->OnMontageEnded.AddDynamic(this, &UBaseCharacterMovementComponent::OnClimbMontageEnded);
		AnimInstance->OnMontageBlendingOut.AddDynamic(this, &UBaseCharacterMovementComponent::OnClimbMontageEnded);
	}
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

void UBaseCharacterMovementComponent::PhysWallRun(float DeltaTime, int32 Iterations)
{
	if (DeltaTime < MIN_TICK_TIME)
	{
		return;
	}

	if (!CharacterOwner || (!CharacterOwner->Controller && !bRunPhysicsWithNoController && !HasAnimRootMotion() && !CurrentRootMotion.HasOverrideVelocity() && (CharacterOwner->GetLocalRole() != ROLE_SimulatedProxy)))
	{
		Acceleration = FVector::ZeroVector;
		Velocity = FVector::ZeroVector;
		return;
	}

	bJustTeleported = false;
	float remainingTime = DeltaTime;
	// Perform the move
	while ((remainingTime >= MIN_TICK_TIME) && (Iterations < MaxSimulationIterations) && CharacterOwner && (CharacterOwner->Controller || bRunPhysicsWithNoController || (CharacterOwner->GetLocalRole() == ROLE_SimulatedProxy)))
	{
		Iterations++;
		bJustTeleported = false;
		const float timeTick = GetSimulationTimeStep(remainingTime, Iterations);
		remainingTime -= timeTick;
		const FVector OldLocation = UpdatedComponent->GetComponentLocation();

		//
		FHitResult WallHit;
		IsWallOnSideTrace(WallHit, Safe_bWallRunIsRight);

		float SinPullAwayAngle = FMath::Sin(FMath::DegreesToRadians(WallRunPullAwayAngle));
		bool bWantsToPullAway = WallHit.IsValidBlockingHit() && !Acceleration.IsNearlyZero() && (Acceleration.GetSafeNormal() | WallHit.Normal) > SinPullAwayAngle;
		if (!WallHit.IsValidBlockingHit() || bWantsToPullAway)
		{
			SetMovementMode(MOVE_Falling);
			StartNewPhysics(remainingTime, Iterations);
			return;
		}

		Acceleration = FVector::VectorPlaneProject(Acceleration, WallHit.Normal);
		Acceleration.Z = 0;

		CalcVelocity(timeTick, 0.f, false, GetMaxBrakingDeceleration());

		Velocity = FVector::VectorPlaneProject(Velocity, WallHit.Normal);

		float TangentAcceleration = Acceleration.GetSafeNormal() | Velocity.GetSafeNormal();
		bool bVelUp = Velocity.Z > 0.f;

		Velocity.Z += GetGravityZ() * WallRunGravityScaleCurve->GetFloatValue(bVelUp ? 0.f : TangentAcceleration) * timeTick;

		if (Velocity.SizeSquared2D() < pow(MinWallRunSpeed, 2) || Velocity.Z < -MaxVerticalWallRunSpeed)
		{
			SetMovementMode(MOVE_Falling);
			StartNewPhysics(remainingTime, Iterations);
			return;
		}

		const FVector Delta = timeTick * Velocity;
		const bool bZeroDelta = Delta.IsNearlyZero();

		if (bZeroDelta)
		{
			remainingTime = 0.f;
		}
		else
		{
			FHitResult Hit;
			SafeMoveUpdatedComponent(Delta, UpdatedComponent->GetComponentQuat(), true, Hit); //line that does all the movement. Keep same capsule rotation. sweep instead teleporting
			FVector	WallAttractionDelta = -WallHit.Normal * WallAttractionForce * timeTick;
			SafeMoveUpdatedComponent(WallAttractionDelta, UpdatedComponent->GetComponentQuat(), true, Hit);

		}

		if (UpdatedComponent->GetComponentLocation() == OldLocation)
		{
			remainingTime = 0.f;
			break;
		}
		Velocity = (UpdatedComponent->GetComponentLocation() - OldLocation) / timeTick; // v = dx / dt . Need for losing velocity while player hits wall
	}

	FHitResult FloorHit, WallHit;
	IsWallOnSideTrace(WallHit, Safe_bWallRunIsRight);
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(GetBaseCharacterOwner());
	GetWorld()->LineTraceSingleByChannel(FloorHit, UpdatedComponent->GetComponentLocation(),
		UpdatedComponent->GetComponentLocation() + FVector::DownVector * (GetBaseCharacterOwner()->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() + MinWallRunHeight * .5f),
		ECC_Visibility, Params);

	if (FloorHit.IsValidBlockingHit() || !WallHit.IsValidBlockingHit() || Velocity.SizeSquared2D() < pow(MinWallRunSpeed, 2))
	{
		SetMovementMode(MOVE_Falling);
	}
}

void UBaseCharacterMovementComponent::PhysClimb(float DeltaTime, int32 Iterations)
{
	if (!bIsHopping)
	{
		GetClimbableSurfaces();
		ProcessClimbableSurfaceInfo();

		if (ShouldStopClimbing() || CheckHasReachedFloor())
		{
			StopClimbing();
		}

		SnapMovementToClimbableSurfaces(DeltaTime);

	}

	RestorePreAdditiveRootMotionVelocity();

	if (!HasAnimRootMotion() && !CurrentRootMotion.HasOverrideVelocity())
	{ // Define the max climb speed and acceleration
		CalcVelocity(DeltaTime, 0.f, true, 400.0f);
	}

	ApplyRootMotionToVelocity(DeltaTime);

	FVector OldLocation = UpdatedComponent->GetComponentLocation();
	const FVector Adjusted = Velocity * DeltaTime;
	FHitResult Hit(1.f);

	SafeMoveUpdatedComponent(Adjusted, GetClimbRotation(DeltaTime), true, Hit);

	if (Hit.Time < 1.f)
	{
		// adjust and try again
		HandleImpact(Hit, DeltaTime, Adjusted);
		SlideAlongSurface(Adjusted, (1.f - Hit.Time), Hit.Normal, Hit, true);
	}

	if (!HasAnimRootMotion() && !CurrentRootMotion.HasOverrideVelocity())
	{
		Velocity = (UpdatedComponent->GetComponentLocation() - OldLocation) / DeltaTime;
	}

	if (CheckHasReachedLedge())
	{
		PlayClimbMontage(ClimbToTopMontage);
	}
}

ABaseCharacter* UBaseCharacterMovementComponent::GetBaseCharacterOwner() const
{
	return StaticCast<ABaseCharacter*>(CharacterOwner);
}

bool UBaseCharacterMovementComponent::IsWallOnSideTrace(FHitResult& WallHit, bool bWallRight) const
{
	const FVector Start = UpdatedComponent->GetComponentLocation();
	const FVector RightTraceDistance = UpdatedComponent->GetRightVector() * CharacterOwner->GetCapsuleComponent()->GetScaledCapsuleRadius() * 2;
	const FVector End = bWallRight ? Start + RightTraceDistance : Start - RightTraceDistance;

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(GetBaseCharacterOwner());
	return TheAfterlife_TraceUtils::SweepCapsuleSingleByChannel(GetWorld(), WallHit, Start, End, 30.0f, 42.0f, FQuat::Identity, ECC_Visibility, Params, FCollisionResponseParams::DefaultResponseParam, true, 5.0f);
	//return GetWorld()->LineTraceSingleByChannel(WallHit, Start, End, ECC_Visibility, Params);
}

bool UBaseCharacterMovementComponent::CanStartClimbing()
{
	if (IsFalling())
		return false;
	ClimbableSurfacesTracedResults = GetClimbableSurfaces();
	if (ClimbableSurfacesTracedResults.IsEmpty())
		return false;
	if (!TraceFromEyeHeight(100.f).bBlockingHit)
		return false;

	return true;
}

void UBaseCharacterMovementComponent::OnClimbMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (Montage == IdleToClimbMontage || Montage == ClimbDownLedgeMontage)
	{
		StartClimbing();
		StopMovementImmediately();
	}
	if (Montage == ClimbToTopMontage)
	{
		SetMovementMode(MOVE_Walking);
	}
	if (Montage == HopRightMontage || Montage == HopLeftMontage)
	{
		bIsHopping = false;
	}
}

TArray<FHitResult> UBaseCharacterMovementComponent::GetClimbableSurfaces()
{
	const FVector& StartOffset = UpdatedComponent->GetForwardVector() * 30.f;
	const FVector& Start = UpdatedComponent->GetComponentLocation() + StartOffset;
	const FVector& End = Start + UpdatedComponent->GetForwardVector();
	TheAfterlife_TraceUtils::SweepCapsuleMultiByChannel(GetWorld(), ClimbableSurfacesTracedResults, Start, End, 30.0f, 72.0f, FQuat::Identity, ECC_Climbing, FCollisionQueryParams::DefaultQueryParam, FCollisionResponseParams::DefaultResponseParam, true);
	return ClimbableSurfacesTracedResults;
}

void UBaseCharacterMovementComponent::ProcessClimbableSurfaceInfo()
{
	CurrentClimbableSurfaceLocation = FVector::ZeroVector;
	CurrentClimbableSurfaceNormal = FVector::ZeroVector;

	if (ClimbableSurfacesTracedResults.IsEmpty())
		return;

	for (const FHitResult& TracedHitResult : ClimbableSurfacesTracedResults)
	{
		CurrentClimbableSurfaceLocation += TracedHitResult.ImpactPoint;
		CurrentClimbableSurfaceNormal += TracedHitResult.ImpactNormal;
	}

	CurrentClimbableSurfaceLocation /= ClimbableSurfacesTracedResults.Num();
	CurrentClimbableSurfaceNormal = CurrentClimbableSurfaceNormal.GetSafeNormal();
}

bool UBaseCharacterMovementComponent::ShouldStopClimbing()
{
	if (ClimbableSurfacesTracedResults.IsEmpty())
		return true;

	const float DotResult = FVector::DotProduct(CurrentClimbableSurfaceNormal, FVector::UpVector);
	const float DegreeDiff = FMath::RadiansToDegrees(FMath::Acos(DotResult));

	if (DegreeDiff <= 60.f)
	{
		return true;
	}

	return false;
}

bool UBaseCharacterMovementComponent::CheckHasReachedFloor()
{
	const FVector DownVector = -UpdatedComponent->GetUpVector();
	const FVector StartOffset = DownVector * 50.f;

	const FVector Start = UpdatedComponent->GetComponentLocation() + StartOffset;
	const FVector End = Start + DownVector;

	FHitResult PossibleFloorHit;
	TheAfterlife_TraceUtils::SweepCapsuleSingleByChannel(GetWorld(), PossibleFloorHit, Start, End, 30.0f, 72.0f, FQuat::Identity, ECC_Visibility);

	if (PossibleFloorHit.IsValidBlockingHit())
	{
		const bool bFloorReached =
			FVector::Parallel(-PossibleFloorHit.ImpactNormal, FVector::UpVector) &&
			GetUnrotatedClimbVelocity().Z < -10.f;

		if (bFloorReached)
		{
			return true;
		}
	}

	return false;
}

bool UBaseCharacterMovementComponent::CheckHasReachedLedge()
{
	FHitResult LedgetHitResult = TraceFromEyeHeight(80.f, 50.f);

	if (!LedgetHitResult.bBlockingHit)
	{
		const FVector WalkableSurfaceTraceStart = LedgetHitResult.TraceEnd;

		const FVector DownVector = -UpdatedComponent->GetUpVector();
		const FVector WalkableSurfaceTraceEnd = WalkableSurfaceTraceStart + DownVector * 100.f;

		FHitResult WalkabkeSurfaceHitResult;
		TheAfterlife_TraceUtils::SweepCapsuleSingleByChannel(GetWorld(), WalkabkeSurfaceHitResult, WalkableSurfaceTraceStart, WalkableSurfaceTraceEnd, 30.0f, 72.0f, FQuat::Identity, ECC_Visibility);

		if (WalkabkeSurfaceHitResult.bBlockingHit && GetUnrotatedClimbVelocity().Z > 10.f)
		{
			return true;
		}
	}

	return false;
}

bool UBaseCharacterMovementComponent::TryWallRun()
{
	if (!IsFalling()) return false;
	if (Velocity.SizeSquared2D() < pow(MinWallRunSpeed, 2)) return false;
	if (Velocity.Z < -MaxVerticalWallRunSpeed) return false; //If player fall very quickly, he can not enter the wall run

	FHitResult FloorHit;
	//Player height check
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(GetBaseCharacterOwner());
	if (GetWorld()->LineTraceSingleByChannel(FloorHit,
		UpdatedComponent->GetComponentLocation(),
		UpdatedComponent->GetComponentLocation() + FVector::DownVector * (CharacterOwner->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() + MinWallRunHeight),
		ECC_Visibility, Params))
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, TEXT("Check"));
		return false;
	}

	//Side casts . Velocity | WallHit.Normal must be less than 90 degrees to be able to enter wall run
	FHitResult WallHit;
	IsWallOnSideTrace(WallHit, false);
	if (WallHit.IsValidBlockingHit())
	{
		Safe_bWallRunIsRight = false;
	}
	else
	{
		IsWallOnSideTrace(WallHit, true);
		if (WallHit.IsValidBlockingHit())
		{
			Safe_bWallRunIsRight = true;
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Purple, TEXT("Check"));
			return false;
		}

	}
	FVector ProjectedVelocity = FVector::VectorPlaneProject(Velocity, WallHit.Normal);
	Velocity = ProjectedVelocity;
	Velocity.Z = FMath::Clamp(Velocity.Z, 0.f, MaxVerticalWallRunSpeed);
	SetMovementMode(MOVE_Custom, (uint8)ECustomMovementMode::CMOVE_WallRun);

	return true;
}

FHitResult UBaseCharacterMovementComponent::TraceFromEyeHeight(float TraceDistance, float TraceStartOffset)
{
	const FVector ComponentLocation = UpdatedComponent->GetComponentLocation();
	const FVector EyeHeightOffset = UpdatedComponent->GetUpVector() * (CharacterOwner->BaseEyeHeight + TraceStartOffset);
	const FVector Start = ComponentLocation + EyeHeightOffset;
	const FVector End = Start + UpdatedComponent->GetForwardVector() * TraceDistance;

	FHitResult OutHit;
	TheAfterlife_TraceUtils::SweepCapsuleSingleByChannel(GetWorld(), OutHit, Start, End, 30.0f, 72.0f, FQuat::Identity, ECC_Climbing);
	return OutHit;
}

FHitResult UBaseCharacterMovementComponent::TraceFromEyeHeightHop(float TraceDistance)
{
	const FVector ComponentLocation = UpdatedComponent->GetComponentLocation();
	const FVector Start = ComponentLocation + UpdatedComponent->GetRightVector() * TraceDistance;
	const FVector End = Start + UpdatedComponent->GetForwardVector() * 100.0f;
	FHitResult OutHit;
	TheAfterlife_TraceUtils::SweepCapsuleSingleByChannel(GetWorld(), OutHit, Start, End, 30.0f, 72.0f, FQuat::Identity, ECC_Climbing);
	return OutHit;
}

void UBaseCharacterMovementComponent::StartClimbing()
{
	SetMovementMode(MOVE_Custom, (uint8)ECustomMovementMode::CMOVE_Parkour);
}

void UBaseCharacterMovementComponent::StopClimbing()
{
	if (IsClimbing())
	{
		SetMovementMode(MOVE_Falling);
	}
	else
	{
		SetMovementMode(MOVE_Walking);
	}
}

void UBaseCharacterMovementComponent::PlayClimbMontage(UAnimMontage* MontageToPlay)
{
	if (!MontageToPlay)
		return;

	UAnimInstance* AnimInstance = CharacterOwner->GetMesh()->GetAnimInstance();

	if (!AnimInstance)
		return;
	if (AnimInstance->IsAnyMontagePlaying())
		return;

	AnimInstance->Montage_Play(MontageToPlay);
}

void UBaseCharacterMovementComponent::SnapMovementToClimbableSurfaces(float DeltaTime)
{
	const FVector ComponentForward = UpdatedComponent->GetForwardVector();
	const FVector ComponentLocation = UpdatedComponent->GetComponentLocation();

	const FVector ProjectedCharacterToSurface =
		(CurrentClimbableSurfaceLocation - ComponentLocation).ProjectOnTo(ComponentForward);

	const FVector SnapVector = -CurrentClimbableSurfaceNormal * ProjectedCharacterToSurface.Length();

	UpdatedComponent->MoveComponent(
		SnapVector * DeltaTime * 100.0f,
		UpdatedComponent->GetComponentQuat(),
		true);
}

FQuat UBaseCharacterMovementComponent::GetClimbRotation(float DeltaTime)
{
	const FQuat CurrentQuat = UpdatedComponent->GetComponentQuat();

	if (HasAnimRootMotion() || CurrentRootMotion.HasOverrideVelocity())
	{
		return CurrentQuat;
	}

	const FQuat TargetQuat = FRotationMatrix::MakeFromX(-CurrentClimbableSurfaceNormal).ToQuat();

	return FMath::QInterpTo(CurrentQuat, TargetQuat, DeltaTime, 5.f);
}

void UBaseCharacterMovementComponent::HandleHopRight()
{
	FVector HopRightTargetPoint;

	if (CheckCanHopRight(HopRightTargetPoint))
	{
		SetMotionWarpTarget(FName("HopRightTargetPoint"), HopRightTargetPoint);

		PlayClimbMontage(HopRightMontage);
		bIsHopping = true;
	}
}

void UBaseCharacterMovementComponent::HandleHopLeft()
{
	FVector HopLeftTargetPoint;

	if (CheckCanHopLeft(HopLeftTargetPoint))
	{
		SetMotionWarpTarget(FName("HopLeftTargetPoint"), HopLeftTargetPoint);

		PlayClimbMontage(HopLeftMontage);
		bIsHopping = true;
	}
}

bool UBaseCharacterMovementComponent::CheckCanHopRight(FVector& OutHopRightTargetPosition)
{
	FHitResult HopRightHit = TraceFromEyeHeightHop(250.f);
	//FHitResult SaftyLedgeHit = TraceFromEyeHeightHop(100.f, 150.f);

	if (HopRightHit.bBlockingHit /* && SaftyLedgeHit.bBlockingHit*/)
	{

		OutHopRightTargetPosition = HopRightHit.ImpactPoint;

		return true;
	}

	return false;
}

bool UBaseCharacterMovementComponent::CheckCanHopLeft(FVector& OutHopLeftTargetPosition)
{
	FHitResult HopLeftHit = TraceFromEyeHeightHop(-250.f);

	if (HopLeftHit.bBlockingHit)
	{

		OutHopLeftTargetPosition = HopLeftHit.ImpactPoint;

		return true;
	}

	return false;
}

void UBaseCharacterMovementComponent::SetMotionWarpTarget(const FName& InWarpTargetName, const FVector& InTargetPosition)
{
	if (!GetBaseCharacterOwner())
		return;

	GetBaseCharacterOwner()->GetMotionWarpingComponent()->AddOrUpdateWarpTargetFromLocation(
		InWarpTargetName,
		InTargetPosition);
}
