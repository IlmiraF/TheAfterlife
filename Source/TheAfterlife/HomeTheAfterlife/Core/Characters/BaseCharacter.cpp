// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "../Components/MovementComponents/BaseCharacterMovementComponent.h"
#include "../Components/CharacterComponents/CharacterAttributeComponent.h"
#include "../Components/AdditionalComponents/LedgeDetectorComponent.h"
#include "Curves/CurveVector.h"
#include "../Actors/Interactive/Environment/Ladder.h"
#include "../Actors/Interactive/Environment/Zipline.h"
#include "../Actors/Interactive/InteractiveActor.h"
#include "../../../TheAfterlifeTypes.h"
#include "../Components/WeaponComponents/MeleeCombatComponent.h"
#include "Components/BoxComponent.h"
#include "Engine/DamageEvents.h"

ABaseCharacter::ABaseCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UBaseCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	BaseCharacterMovementComponent = StaticCast<UBaseCharacterMovementComponent*>(GetCharacterMovement());

	LedgeDetectorComponent = CreateDefaultSubobject<ULedgeDetectorComponent>(TEXT("Ledge Detector"));

	GetMesh()->CastShadow = true;
	GetMesh()->bCastDynamicShadow = true;

	CharacterAttributesComponent = CreateDefaultSubobject<UCharacterAttributeComponent>(TEXT("Attribute Component"));
	MeleeCombatComponent = CreateDefaultSubobject<UMeleeCombatComponent>(TEXT("Melee combat Component"));

	LeftHandCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("LeftHandCollisionBox"));
	RightHandCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("RightHandCollisionBox"));

	LeftHandCollision->SetupAttachment(GetRootComponent());
	LeftHandCollision->SetHiddenInGame(false);
	LeftHandCollision->SetCollisionProfileName("NoCollision");
	LeftHandCollision->SetNotifyRigidBodyCollision(false);

	RightHandCollision->SetupAttachment(GetRootComponent());
	RightHandCollision->SetHiddenInGame(false);
	RightHandCollision->SetCollisionProfileName("NoCollision");
	RightHandCollision->SetNotifyRigidBodyCollision(false);
}

void ABaseCharacter::Jump()
{
	Super::Jump();
}

void ABaseCharacter::ChangeCrouchState()
{
	if (!GetMovementComponent()->IsCrouching())
	{
		Crouch();
	}
	else
	{
		UnCrouch();
	}
}

void ABaseCharacter::Falling()
{
	Super::Falling();
	GetBaseCharacterMovementComponent()->bNotifyApex = true;
}

void ABaseCharacter::NotifyJumpApex()
{
	Super::NotifyJumpApex();
	CurrentFallApex = GetActorLocation();
}

void ABaseCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);
	float FallHeight = (CurrentFallApex - GetActorLocation()).Z * 0.01f;
	if (IsValid(FallDamageCurve))
	{
		float DamageAmount = FallDamageCurve->GetFloatValue(FallHeight);
		TakeDamage(DamageAmount, FDamageEvent(), GetController(), Hit.GetActor());
	}
}

void ABaseCharacter::Mantle(bool bForce)
{
	if (!(CanMantle() || bForce))
	{
		return;
	}

	FLedgeDescription LedgeDescription;
	if (LedgeDetectorComponent->DetectLedge(LedgeDescription) && !GetBaseCharacterMovementComponent()->IsMantling())
	{
		JumpCount = 0;

		bIsMantling = true;

		FMantlingMovementParameters MantlingParameters;
		MantlingParameters.InitialLocation = GetActorLocation();
		MantlingParameters.InitialRotation = GetActorRotation();
		MantlingParameters.TargetLocation = LedgeDescription.Location;
		MantlingParameters.TargetRotation = LedgeDescription.Rotation;

		float MantlingHeight = (MantlingParameters.TargetLocation - MantlingParameters.InitialLocation).Z;
		const FMantlingSettings& MantlingSettings = GetMantlingSettings(MantlingHeight);

		float MinRange;
		float MaxRange;
		MantlingSettings.MantlingCurve->GetTimeRange(MinRange, MaxRange);

		MantlingParameters.Duration = MaxRange - MinRange;

		MantlingParameters.MantlingCurve = MantlingSettings.MantlingCurve;

		FVector2D SourceRange(MantlingSettings.MinHeight, MantlingSettings.MaxHeight);
		FVector2D TargetRange(MantlingSettings.MinHeightStartTime, MantlingSettings.MaxHeightStartTime);
		MantlingParameters.StartTime = FMath::GetMappedRangeValueClamped(SourceRange, TargetRange, MantlingHeight);

		MantlingParameters.InitialAnimationLocation = MantlingParameters.TargetLocation - MantlingSettings.AnimationCorrectionZ * FVector::UpVector + MantlingSettings.AnimationCorrectionXY * LedgeDescription.LedgeNormal;

		GetBaseCharacterMovementComponent()->StartMantle(MantlingParameters);

		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		AnimInstance->Montage_Play(MantlingSettings.MantlingMontage, 1.0f, EMontagePlayReturnType::Duration, MantlingParameters.StartTime);
	}
}

void ABaseCharacter::RegisterInteractiveActor(AInteractiveActor* InteractiveActor)
{
	AvailableInteractiveActors.AddUnique(InteractiveActor);
}

void ABaseCharacter::UnregisterInteractiveActor(AInteractiveActor* InteractiveActor)
{
	AvailableInteractiveActors.RemoveSingleSwap(InteractiveActor);
}

void ABaseCharacter::ClimbLadderUp(float Value)
{
	if (GetBaseCharacterMovementComponent()->IsOnLadder() && !FMath::IsNearlyZero(Value))
	{
		FVector LadderUpVector = GetBaseCharacterMovementComponent()->GetCurrentLadder()->GetActorUpVector();
		AddMovementInput(LadderUpVector, Value);
	}
}

void ABaseCharacter::InteractWithLadder()
{
	if (GetBaseCharacterMovementComponent()->IsOnLadder())
	{
		GetBaseCharacterMovementComponent()->DetachFromLadder(EDetachFromLadderMethod::JumpOff);
	}
	else
	{
		const ALadder* AvailableLadder = GetAvailableLadder();
		if (IsValid(AvailableLadder))
		{
			if (AvailableLadder->GetIsOnTop())
			{
				PlayAnimMontage(AvailableLadder->GetAttachFromTopAnimMontage());
			}
			GetBaseCharacterMovementComponent()->AttachToLadder(AvailableLadder);
		}
	}
}

const ALadder* ABaseCharacter::GetAvailableLadder() const
{
	const ALadder* Result = nullptr;
	for (const AInteractiveActor* InteractiveActor : AvailableInteractiveActors)
	{
		if (InteractiveActor->IsA<ALadder>())
		{
			Result = StaticCast<const ALadder*>(InteractiveActor);
			break;
		}
	}
	return Result;
}

void ABaseCharacter::InteractWithZipline()
{
	if (GetBaseCharacterMovementComponent()->IsOnZipline())
	{
		GetBaseCharacterMovementComponent()->DetachFromZipline();
	}
	else
	{
		const AZipline* AvailableZipline = GetAvailableZipline();
		if (IsValid(AvailableZipline))
		{
			GetBaseCharacterMovementComponent()->AttachToZipline(AvailableZipline);
		}
	}
}

const AZipline* ABaseCharacter::GetAvailableZipline() const
{
	const AZipline* Result = nullptr;
	for (const AInteractiveActor* InteractiveActor : AvailableInteractiveActors)
	{
		if (InteractiveActor->IsA<AZipline>())
		{
			Result = StaticCast<const AZipline*>(InteractiveActor);
			break;
		}
	}
	return Result;
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	const FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, false);

	LeftHandCollision->AttachToComponent(GetMesh(), AttachmentRules, "hand_left_collision");
	RightHandCollision->AttachToComponent(GetMesh(), AttachmentRules, "hand_right_collision");

	LeftHandCollision->OnComponentHit.AddDynamic(this, &ABaseCharacter::OnAttackHit);
	//LeftHandCollision->OnComponentBeginOverlap.AddDynamic(this, &ABaseCharacter::OnAttackOverlapBegin);
	//LeftHandCollision->OnComponentEndOverlap.AddDynamic(this, &ABaseCharacter::OnAttackOverlapEnd);

	RightHandCollision->OnComponentHit.AddDynamic(this, &ABaseCharacter::OnAttackHit);
	//RightHandCollision->OnComponentBeginOverlap.AddDynamic(this, &ABaseCharacter::OnAttackOverlapBegin);
	//RightHandCollision->OnComponentEndOverlap.AddDynamic(this, &ABaseCharacter::OnAttackOverlapEnd);
}

bool ABaseCharacter::CanMantle() const
{
	return !GetBaseCharacterMovementComponent()->IsOnLadder() && !GetBaseCharacterMovementComponent()->IsOnZipline();
}

void ABaseCharacter::OnDeath()
{
	GetCharacterMovement()->DisableMovement();
	float Duration = PlayAnimMontage(OnDeathAnimMontage);
	if (Duration == 0.0f)
	{
		EnableRagdoll();
	}
}

const FMantlingSettings& ABaseCharacter::GetMantlingSettings(float LedgeHeight) const
{
	return LedgeHeight > LowMantleMaxHeight ? HighMantleSettings : LowMantleSettings;
}

void ABaseCharacter::EnableRagdoll()
{
	GetMesh()->SetCollisionProfileName(CollisionProfileRagdoll);
	GetMesh()->SetSimulatePhysics(true);
}

void ABaseCharacter::AttackInput()
{
	int MontageSectionIndex = rand() % 2 + 1;
	FString MontageSection = "Start_" + FString::FromInt(MontageSectionIndex);

	PlayAnimMontage(MeleeCombatMontage, 1.0f, FName(MontageSection));
}

void ABaseCharacter::MeleeAttackStart()
{
	//MeleeCombatComponent->MeleeAttackStart();

	LeftHandCollision->SetCollisionProfileName("Weapon");
	LeftHandCollision->SetNotifyRigidBodyCollision(true);
	//LeftHandCollision->SetGenerateOverlapEvents(true);

	RightHandCollision->SetCollisionProfileName("Weapon");
	RightHandCollision->SetNotifyRigidBodyCollision(true);
	//RightHandCollision->SetGenerateOverlapEvents(true);

	GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::White, FString::Printf(TEXT("AttackStart")));
}

void ABaseCharacter::MeleeAttackFinish()
{
	//MeleeCombatComponent->MeleeAttackFinish();

	LeftHandCollision->SetCollisionProfileName("NoCollision");
	LeftHandCollision->SetNotifyRigidBodyCollision(false);
	//LeftHandCollision->SetGenerateOverlapEvents(false);

	RightHandCollision->SetCollisionProfileName("NoCollision");
	RightHandCollision->SetNotifyRigidBodyCollision(false);
	//RightHandCollision->SetGenerateOverlapEvents(false);

	GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Black, FString::Printf(TEXT("AttackFinish")));
}

void ABaseCharacter::OnAttackHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Yellow, Hit.GetActor()->GetName());
}

//void ABaseCharacter::OnAttackOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
//{
//	GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Red, OtherActor->GetName());
//}
//
//void ABaseCharacter::OnAttackOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
//{
//	GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Black, OtherActor->GetName());
//}
