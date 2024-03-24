// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacterMovementComponent.h"
#include "../../Characters/BaseCharacter.h"
#include "Curves/CurveVector.h"
#include "../../Actors/Interactive/Environment/Ladder.h"
#include "../../Actors/ParkourLedge.h"
#include "Kismet/KismetMathLibrary.h"
#include "../../Utils/TheAfterlife_TraceUtils.h"
#include "MotionWarpingComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/CapsuleComponent.h"
#include "../../../../TheAfterlifeTypes.h"

float UBaseCharacterMovementComponent::GetMaxSpeed() const
{
    float Result = Super::GetMaxSpeed();
	if (IsOnLadder())
	{
		Result = ClimbingOnLadderMaxSpeed;
	}
	if (IsWallRunning())
	{
		Result = MaxWallRunSpeed;
	}
	if (IsClimbing())
	{
		Result = ClimbMaxSpeed;
	}
	if (IsOnBeam())
	{
		Result = OnBeamMaxSpeed;
	}
	if (GetBaseCharacterOwner()->IsAiming())
	{
		Result = GetBaseCharacterOwner()->GetAimingMovementSpeed();
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
			PlayClimbMontage(IdleToClimbMontage);
		}
	}

	if (!bAttemptClimbing)
	{
		StopClimbing();
	}
}

void UBaseCharacterMovementComponent::RequestHopping()
{
	const FVector UnrotatedLastInputVector =
		UKismetMathLibrary::Quat_UnrotateVector(UpdatedComponent->GetComponentQuat(), GetLastInputVector());

	const float DotResultRight =
		FVector::DotProduct(UnrotatedLastInputVector.GetSafeNormal(), FVector::RightVector);

	const float DotResultForward =
		FVector::DotProduct(UnrotatedLastInputVector.GetSafeNormal(), FVector::UpVector);

	if (FMath::IsNearlyZero(DotResultRight * DotResultForward, 1e-6f))
	{
		if (DotResultForward >= 0.9f)
		{
			HandleHopUp();
		}
		else if (DotResultForward <= -0.9f)
		{
			HandleHopDown();
		}

		if (DotResultRight >= 0.9f)
		{
			HandleHopRight();
		}
		else if (DotResultRight <= -0.9f)
		{
			HandleHopLeft();
		}
	}

	/*if (DotResultForward >= 0.9f)
	{
		if (DotResultRight >= 0.9f)
		{
			HandleHopRightUp();
		}
		else if (DotResultRight <= -0.9f)
		{
			HandleHopLeftUp();
		}
	}
	else if (DotResultForward <= -0.9f)
	{
		if (DotResultRight >= 0.9f)
		{
			HandleHopRightDown();
		}
		else if (DotResultRight <= -0.9f)
		{
			HandleHopLeftDown();
		}
	}*/
}

bool UBaseCharacterMovementComponent::IsOnBeam() const
{
	return UpdatedComponent && MovementMode == MOVE_Custom && CustomMovementMode == (uint8)ECustomMovementMode::CMOVE_OnBeam;
}

void UBaseCharacterMovementComponent::StartWalkingOnBeam()
{
	SetMovementMode(MOVE_Custom, (uint8)ECustomMovementMode::CMOVE_OnBeam);
	StartBalancingDirection = UKismetMathLibrary::RandomBool() ? 1.0f : -1.0f;
	//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("StartBeam"));
}

void UBaseCharacterMovementComponent::StopWalkingOnBeam()
{
	OnBeamDirection = 0.0f;
	SetMovementMode(MOVE_Walking);
	//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, TEXT("StopBeam"));
}

float UBaseCharacterMovementComponent::GetOnBeamDirection() const
{
	return OnBeamDirection;
}

void UBaseCharacterMovementComponent::SetOnBeamDirection(float Direction)
{
	OnBeamDirection = Direction;
}

void UBaseCharacterMovementComponent::SetBalancingDirection(float Direction)
{
	StartBalancingDirection = Direction;
}

void UBaseCharacterMovementComponent::OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PreviousMovementMode, PreviousCustomMode);

	if (PreviousMovementMode == MOVE_Custom && PreviousCustomMode == (uint8)ECustomMovementMode::CMOVE_Ladder)
	{
		CurrentLadder = nullptr;
	}

	if (PreviousMovementMode == MOVE_Custom && PreviousCustomMode == (uint8)ECustomMovementMode::CMOVE_Parkour)
	{
		bOrientRotationToMovement = true;
		CharacterOwner->GetCapsuleComponent()->SetCapsuleHalfHeight(96.f);

		const FRotator DirtyRotation = UpdatedComponent->GetComponentRotation();
		const FRotator CleanStandRotation = FRotator(0.f, DirtyRotation.Yaw, 0.f);
		UpdatedComponent->SetRelativeRotation(CleanStandRotation);

		StopMovementImmediately();
	}

	if (PreviousMovementMode == MOVE_Falling)
	{
		GetWorld()->GetTimerManager().ClearTimer(FallingTimer);
	}

	if (MovementMode == MOVE_Falling)
	{
		GetWorld()->GetTimerManager().SetTimer(FallingTimer, this, &UBaseCharacterMovementComponent::BackToLastSave, MaxFallingTime);
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
			bOrientRotationToMovement = false;
			CharacterOwner->GetCapsuleComponent()->SetCapsuleHalfHeight(48.f);
			break;
		}
		case (uint8)ECustomMovementMode::CMOVE_OnBeam:
		{
			//bOrientRotationToMovement = false;
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
	case (uint8)ECustomMovementMode::CMOVE_OnBeam:
	{
		PhysBeam(DeltaTime, Iterations);
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
	if ((ShouldStopClimbing() || CheckHasReachedFloor()) && !bIsHopping)
	{
		StopClimbing();
	}

	SnapMovementToClimbableSurfaces(DeltaTime);


	RestorePreAdditiveRootMotionVelocity();

	if (!HasAnimRootMotion() && !CurrentRootMotion.HasOverrideVelocity())
	{
		CalcVelocity(DeltaTime, 0.f, true, 400.0f);
	}

	ApplyRootMotionToVelocity(DeltaTime);

	FVector OldLocation = UpdatedComponent->GetComponentLocation();
	const FVector Adjusted = Velocity * DeltaTime;
	FHitResult Hit(1.f);

	SafeMoveUpdatedComponent(Adjusted, GetClimbRotation(DeltaTime), true, Hit);

	if (Hit.Time < 1.f)
	{
		HandleImpact(Hit, DeltaTime, Adjusted);
		SlideAlongSurface(Adjusted, (1.f - Hit.Time), Hit.Normal, Hit, true);
	}

	if (!HasAnimRootMotion() && !CurrentRootMotion.HasOverrideVelocity())
	{
		Velocity = (UpdatedComponent->GetComponentLocation() - OldLocation) / DeltaTime;
	}
}

void UBaseCharacterMovementComponent::PhysBeam(float DeltaTime, int32 Iterations)
{
	CalcVelocity(DeltaTime, 1.0f, false, ClimbingOnLadderBrakingDecelaration);
	FVector Delta = Velocity * DeltaTime;

	OnBeamDirection += DeltaTime * 10.0f * StartBalancingDirection;

	//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Orange, FString::Printf(TEXT("%f"), OnBeamDirection));

	if (FMath::Abs(OnBeamDirection) >= 45.0f)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Orange, FString::Printf(TEXT("%f"), OnBeamDirection));
		//StopWalkingOnBeam();
		FVector Start = UpdatedComponent->GetComponentLocation();
		FVector FallDirection = Start - StartBalancingDirection * UpdatedComponent->GetRightVector() * 100.0f;
		GetOwner()->SetActorLocation(FallDirection);
		SetMovementMode(MOVE_Falling);
	}

	FHitResult Hit;
	SafeMoveUpdatedComponent(Delta, UpdatedComponent->GetComponentRotation(), false, Hit);
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
	return TheAfterlife_TraceUtils::SweepCapsuleSingleByChannel(GetWorld(), WallHit, Start, End, 30.0f, 42.0f, FQuat::Identity, ECC_Visibility, Params, FCollisionResponseParams::DefaultResponseParam, false, 5.0f);
	//return GetWorld()->LineTraceSingleByChannel(WallHit, Start, End, ECC_Visibility, Params);
}

bool UBaseCharacterMovementComponent::CanStartClimbing()
{
	if (IsFalling())
		return false;
	ClimbableSurfacesTracedResults = GetClimbableSurfaces();
	if (!ClimbableSurfacesTracedResults.bBlockingHit)
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
	if (Montage == HopRightMontage || Montage == HopLeftMontage || Montage == HopUpMontage || Montage == HopDownMontage)
	{
		bIsHopping = false;
	}

	if (Montage == HopUpMontage)
	{
		if (CheckHasReachedLedge())
		{
			GetBaseCharacterOwner()->Mantle(true);
		}
	}
}

FHitResult UBaseCharacterMovementComponent::GetClimbableSurfaces()
{
	const FVector ComponentLocation = UpdatedComponent->GetComponentLocation();
	const FVector Start = ComponentLocation + UpdatedComponent->GetUpVector() * 100.0f;
	const FVector End = Start + UpdatedComponent->GetForwardVector() * 30.0f;
	FHitResult HitRes;
	FCollisionShape CollisionShape = FCollisionShape::MakeSphere(ClimbCapsuleTraceRadius);
	GetWorld()->SweepSingleByObjectType(HitRes, Start, End, FQuat::Identity, ECC_Parkour, CollisionShape);
	//DrawDebugSphere(GetWorld(), HitRes.ImpactPoint, ClimbCapsuleTraceRadius, 16, FColor::Purple, false, 2.0f);
	ClimbableSurfacesTracedResults = HitRes;
	return ClimbableSurfacesTracedResults;
}

void UBaseCharacterMovementComponent::ProcessClimbableSurfaceInfo()
{
	CurrentClimbableSurfaceLocation = ClimbableSurfacesTracedResults.ImpactPoint;
	CurrentClimbableSurfaceNormal = ClimbableSurfacesTracedResults.ImpactNormal.GetSafeNormal();
}

bool UBaseCharacterMovementComponent::ShouldStopClimbing()
{
	if (!ClimbableSurfacesTracedResults.bBlockingHit)
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

	TArray<TEnumAsByte<EObjectTypeQuery>> Types;
	Types.Add(EObjectTypeQuery::ObjectTypeQuery1);
	TArray<FHitResult> PossibleFloorHits = TheAfterlife_TraceUtils::SweepCapsuleMultiByObjectType(GetWorld(), Start, End, Types);

	if (PossibleFloorHits.IsEmpty())
		return false;

	for (const FHitResult& PossibleFloorHit : PossibleFloorHits)
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

	FHitResult OutHit = TheAfterlife_TraceUtils::LineTraceSingleByObject(GetWorld(), Start, End, ClimbableSurfaceTraceTypes);

	return OutHit;
}

void UBaseCharacterMovementComponent::StartClimbing()
{
	SetMovementMode(MOVE_Custom, (uint8)ECustomMovementMode::CMOVE_Parkour);
	ProcessClimbableSurfaceInfo();
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
	const FVector ComponentLocation = UpdatedComponent->GetComponentLocation();
	const FVector ComponentForward = UpdatedComponent->GetForwardVector();

	const FVector ProjectedCharacterToSurface =
		(CurrentClimbableSurfaceLocation - ComponentLocation).ProjectOnTo(ComponentForward);

	const FVector SnapVector = -CurrentClimbableSurfaceNormal * ProjectedCharacterToSurface.Length();

	UpdatedComponent->MoveComponent(SnapVector * DeltaTime * 100.0f, UpdatedComponent->GetComponentQuat(), true);
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

void UBaseCharacterMovementComponent::HandleHopUp()
{
	FVector HopUpTargetPoint;

	if (CheckCanHopUp(HopUpTargetPoint))
	{
		SetMotionWarpTarget(FName("HopUpTargetPoint"), HopUpTargetPoint);

		PlayClimbMontage(HopUpMontage);
		bIsHopping = true;
	}
}

void UBaseCharacterMovementComponent::HandleHopDown()
{
	FVector HopDownTargetPoint;

	if (CheckCanHopDown(HopDownTargetPoint))
	{
		SetMotionWarpTarget(FName("HopDownTargetPoint"), HopDownTargetPoint);

		PlayClimbMontage(HopDownMontage);
		bIsHopping = true;
	}
}

bool UBaseCharacterMovementComponent::CheckCanHopUp(FVector& OutHopUpTargetPosition)
{
	FHitResult HopHit;
	const FVector ComponentLocation = UpdatedComponent->GetComponentLocation();
	const FVector Start = ComponentLocation + UpdatedComponent->GetUpVector() * 240.0f;
	const FVector End = Start + UpdatedComponent->GetForwardVector() * 30.0f;
	FCollisionShape CollisionShape = FCollisionShape::MakeSphere(ClimbCapsuleTraceRadius);
	GetWorld()->SweepSingleByObjectType(HopHit, Start, End, FQuat::Identity, ECC_Parkour, CollisionShape);
	//DrawDebugSphere(GetWorld(), HopHit.ImpactPoint, ClimbCapsuleTraceRadius, 16, FColor::Cyan, false, 2.0f);

	if (HopHit.bBlockingHit)
	{
		const AActor* CurrentLedge = HopHit.GetActor();
		if (CurrentLedge->IsA<AParkourLedge>())
		{
			const AParkourLedge* Ledge = StaticCast<const AParkourLedge*>(CurrentLedge);
			OutHopUpTargetPosition = Ledge->GetUpPointOfLocation();
			CurrentClimbableSurfaceLocation = OutHopUpTargetPosition;
			return true;
		}
	}

	return false;
}

bool UBaseCharacterMovementComponent::CheckCanHopRight(FVector& OutHopRightTargetPosition)
{
	FHitResult HopHit;
	const FVector ComponentLocation = UpdatedComponent->GetComponentLocation();
	const FVector Start = ComponentLocation + UpdatedComponent->GetUpVector() * 80.0f + UpdatedComponent->GetRightVector() * 300.0f;
	const FVector End = Start + UpdatedComponent->GetForwardVector() * 30.0f;
	FCollisionShape CollisionShape = FCollisionShape::MakeSphere(ClimbCapsuleTraceRadius);
	GetWorld()->SweepSingleByObjectType(HopHit, Start, End, FQuat::Identity, ECC_Parkour, CollisionShape);
	//DrawDebugSphere(GetWorld(), HopHit.TraceEnd, ClimbCapsuleTraceRadius, 16, FColor::Cyan, false, 2.0f);

	if (HopHit.bBlockingHit)
	{
		const AActor* CurrentLedge = HopHit.GetActor();
		if (CurrentLedge->IsA<AParkourLedge>())
		{
			const AParkourLedge* Ledge = StaticCast<const AParkourLedge*>(CurrentLedge);
			OutHopRightTargetPosition = Ledge->GetUpPointOfLocation();
			CurrentClimbableSurfaceLocation = OutHopRightTargetPosition;
			return true;
		}
	}

	return false;
}

bool UBaseCharacterMovementComponent::CheckCanHopLeft(FVector& OutHopLeftTargetPosition)
{
	FHitResult HopHit;
	const FVector ComponentLocation = UpdatedComponent->GetComponentLocation();
	const FVector Start = ComponentLocation + UpdatedComponent->GetUpVector() * 80.0f - UpdatedComponent->GetRightVector() * 300.0f;
	const FVector End = Start + UpdatedComponent->GetForwardVector() * 30.0f;
	FCollisionShape CollisionShape = FCollisionShape::MakeSphere(ClimbCapsuleTraceRadius);
	GetWorld()->SweepSingleByObjectType(HopHit, Start, End, FQuat::Identity, ECC_Parkour, CollisionShape);
	//DrawDebugSphere(GetWorld(), HopHit.ImpactPoint, ClimbCapsuleTraceRadius, 16, FColor::Cyan, false, 2.0f);

	if (HopHit.bBlockingHit)
	{
		const AActor* CurrentLedge = HopHit.GetActor();
		if (CurrentLedge->IsA<AParkourLedge>())
		{
			const AParkourLedge* Ledge = StaticCast<const AParkourLedge*>(CurrentLedge);
			OutHopLeftTargetPosition = Ledge->GetUpPointOfLocation();
			CurrentClimbableSurfaceLocation = OutHopLeftTargetPosition;
			return true;
		}
	}

	return false;
}

bool UBaseCharacterMovementComponent::CheckCanHopDown(FVector& OutHopUpTargetPosition)
{
	FHitResult HopHit;
	const FVector ComponentLocation = UpdatedComponent->GetComponentLocation();
	const FVector Start = ComponentLocation - UpdatedComponent->GetUpVector() * 100.0f;
	const FVector End = Start + UpdatedComponent->GetForwardVector() * 30.0f;
	FCollisionShape CollisionShape = FCollisionShape::MakeSphere(ClimbCapsuleTraceRadius);
	GetWorld()->SweepSingleByObjectType(HopHit, Start, End, FQuat::Identity, ECC_Parkour, CollisionShape);
	//DrawDebugSphere(GetWorld(), HopHit.TraceEnd, ClimbCapsuleTraceRadius, 16, FColor::Cyan, false, 2.0f);

	if (HopHit.bBlockingHit)
	{
		const AActor* CurrentLedge = HopHit.GetActor();
		if (CurrentLedge->IsA<AParkourLedge>())
		{
			const AParkourLedge* Ledge = StaticCast<const AParkourLedge*>(CurrentLedge);
			OutHopUpTargetPosition = Ledge->GetDownPointOfLocation();
			CurrentClimbableSurfaceLocation = OutHopUpTargetPosition;
			return true;
		}
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

void UBaseCharacterMovementComponent::BackToLastSave()
{
	GetBaseCharacterOwner()->OnFalling.ExecuteIfBound();
}
