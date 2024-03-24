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
	CMOVE_Ladder UMETA(DisplayName = "Ladder"),
	CMOVE_WallRun	UMETA(DisplayName = "WallRun"),
	CMOVE_Parkour UMETA(DisplayName = "Parkour"),
	CMOVE_OnBeam UMETA(DisplayName = "OnBeam"),
	CMOVE_Max UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EDetachFromLadderMethod : uint8
{
	Fall = 0,
	ReachingTheTop,
	ReachingTheBottom,
	JumpOff
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

	void AttachToLadder(const class ALadder* Ladder);
	void DetachFromLadder(EDetachFromLadderMethod DetachFromLadderMethod = EDetachFromLadderMethod::Fall);
	bool IsOnLadder() const;
	const class ALadder* GetCurrentLadder();
	float GetLadderSpeedRatio() const;
	float GetActorToCurrentLadderProjection(const FVector& Location) const;

	virtual void PhysicsRotation(float DeltaTime) override;

	bool TryWallRun();
	bool IsWallRunning() const;
	bool IsWallRunningRight() const { return Safe_bWallRunIsRight; }

	bool IsClimbing() const;
	bool CanStartClimbing();
	FORCEINLINE FVector GetClimbableSurfaceNormal() const { return CurrentClimbableSurfaceNormal; }
	FVector GetUnrotatedClimbVelocity() const;
	void ToggleClimbing(bool bAttemptClimbing);
	void RequestHopping();

	bool IsOnBeam() const;
	void StartWalkingOnBeam();
	void StopWalkingOnBeam();
	float GetOnBeamDirection() const;
	void SetOnBeamDirection(float Direction);
	void SetBalancingDirection(float Direction);
	
protected:

	virtual void OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode) override;

	virtual void PhysCustom(float DeltaTime, int32 Iterations) override;
	virtual void BeginPlay() override;

	void PhysMantling(float DeltaTime, int32 Iterations);
	void PhysLadder(float DeltaTime, int32 Iterations);
	void PhysWallRun(float DeltaTime, int32 Iterations);
	void PhysClimb(float DeltaTime, int32 Iterations);
	void PhysBeam(float DeltaTime, int32 Iterations);


	class ABaseCharacter* GetBaseCharacterOwner() const;

	UPROPERTY(Category = "Character Movement: Ladder", EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", UIMin = "0"))
	float ClimbingOnLadderMaxSpeed = 200.0f;

	UPROPERTY(Category = "Character Movement: Ladder", EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", UIMin = "0"))
	float ClimbingOnLadderBrakingDecelaration = 2048.0f;

	UPROPERTY(Category = "Character Movement: Ladder", EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", UIMin = "0"))
	float LadderToCharacterOffset = 60.0f;

	UPROPERTY(Category = "Character Movement: Ladder", EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", UIMin = "0"))
	float MaxLadderTopOffset = 90.0f;

	UPROPERTY(Category = "Character Movement: Ladder", EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", UIMin = "0"))
	float MinLadderBottomOffset = 90.0f;

	UPROPERTY(Category = "Character Movement: Ladder", EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", UIMin = "0"))
	float JumpOffFromLadderSpeed = 500.0f;



	UPROPERTY(EditDefaultsOnly, Category = "Character Movement: WallRun") 
	float MinWallRunSpeed = 300.f;

	UPROPERTY(EditDefaultsOnly, Category = "Character Movement: WallRun") 
	float MaxWallRunSpeed = 800.f;

	UPROPERTY(EditDefaultsOnly, Category = "Character Movement: WallRun") 
	float MaxVerticalWallRunSpeed = 180.f; //?

	UPROPERTY(EditDefaultsOnly, Category = "Character Movement: WallRun") 
	float MinWallRunHeight = 10.f;

	UPROPERTY(EditDefaultsOnly, Category = "Character Movement: WallRun") 
	float WallJumpOfForce = 400.f; //Force with which a player jumps away from a wall

	UPROPERTY(EditDefaultsOnly, Category = "Character Movement: WallRun") 
	float WallRunPullAwayAngle = 75.f;

	UPROPERTY(EditDefaultsOnly, Category = "Character Movement: WallRun") 
	float WallAttractionForce = 200.f;

	UPROPERTY(EditDefaultsOnly, Category = "Character Movement: WallRun") 
	UCurveFloat* WallRunGravityScaleCurve;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Movement: Climbing");
	TArray<TEnumAsByte<EObjectTypeQuery>> ClimbableSurfaceTraceTypes;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Movement: Climbing");
	float ClimbCapsuleTraceRadius = 30.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Movement: Climbing");
	float ClimbMaxSpeed = 100.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Movement: Climbing")
	UAnimMontage* IdleToClimbMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Movement: Climbing")
	UAnimMontage* ClimbToTopMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Movement: Climbing")
	UAnimMontage* ClimbDownLedgeMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Movement: Climbing")
	UAnimMontage* HopRightMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Movement: Climbing")
	UAnimMontage* HopLeftMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Movement: Climbing")
	UAnimMontage* HopUpMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Movement: Climbing")
	UAnimMontage* HopDownMontage;


	UPROPERTY(Category = "Character Movement: On Beam", EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", UIMin = "0"))
	float OnBeamMaxSpeed = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character Movement: Falling")
	float MaxFallingTime = 5.0f;

private:
	FMantlingMovementParameters CurrentMantlingParameters;

	FTimerHandle MantlingTimer;

	const ALadder* CurrentLadder = nullptr;

	FRotator ForceTargetRotation = FRotator::ZeroRotator;
	bool bForceRotation = false;

	//Wall Run

	bool IsWallOnSideTrace(FHitResult& WallHit, bool bWallRight) const;
	bool Safe_bWallRunIsRight;

	//Climb

	UFUNCTION()
	void OnClimbMontageEnded(UAnimMontage* Montage, bool bInterrupted);
	bool bIsHopping = false;
	FVector CurrentClimbableSurfaceNormal = FVector::ZeroVector;
	FVector CurrentClimbableSurfaceLocation = FVector::ZeroVector;
	FHitResult ClimbableSurfacesTracedResults;
	FHitResult GetClimbableSurfaces();
	void ProcessClimbableSurfaceInfo();
	bool ShouldStopClimbing();
	bool CheckHasReachedFloor();
	bool CheckHasReachedLedge();
	FHitResult TraceFromEyeHeight(float TraceDistance, float TraceStartOffset = 0.f);
	void StartClimbing();
	void StopClimbing();

	void PlayClimbMontage(UAnimMontage* MontageToPlay);

	void SnapMovementToClimbableSurfaces(float DeltaTime);
	FQuat GetClimbRotation(float DeltaTime);

	void HandleHopRight();
	void HandleHopLeft();
	void HandleHopUp();
	void HandleHopDown();
	bool CheckCanHopUp(FVector& OutHopUpTargetPosition);
	bool CheckCanHopRight(FVector& OutHopUpTargetPosition);
	bool CheckCanHopLeft(FVector& OutHopUpTargetPosition);
	bool CheckCanHopDown(FVector& OutHopUpTargetPosition);
	void SetMotionWarpTarget(const FName& InWarpTargetName, const FVector& InTargetPosition);

	//Beam

	float StartBalancingDirection;
	float OnBeamDirection = 0.0f;

	FTimerHandle FallingTimer;
	void BackToLastSave();
};
