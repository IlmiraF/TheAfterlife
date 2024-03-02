// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "../Components/MovementComponents/BaseCharacterMovementComponent.h"
#include "../Components/CharacterComponents/CharacterAttributeComponent.h"
#include "../Components/CharacterComponents/CharacterEquipmentComponent.h"
#include "../Components/AdditionalComponents/LedgeDetectorComponent.h"
#include "Curves/CurveVector.h"
#include "../Actors/Interactive/Environment/Ladder.h"
#include "../Actors/Interactive/Environment/RunWall.h"
#include "../Actors/Interactive/Environment/Beam.h"
#include "../Actors/Interactive/InteractiveActor.h"
#include "../../../TheAfterlifeTypes.h"
#include "../Components/WeaponComponents/MeleeCombatComponent.h"
#include "../Components/WeaponComponents/MeleeHitRegistrator.h"
#include "Components/BoxComponent.h"
#include "Engine/DamageEvents.h"
#include "MotionWarpingComponent.h"
#include "../Actors/Equipment/Weapons/MeleeWeaponItem.h"
#include "../Actors/Equipment/Weapons/RangeWeaponItem.h"
#include "../Actors/Equipment/Throwables/ThrowableItem.h"
#include "AIController.h"

ABaseCharacter::ABaseCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UBaseCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	BaseCharacterMovementComponent = StaticCast<UBaseCharacterMovementComponent*>(GetCharacterMovement());

	LedgeDetectorComponent = CreateDefaultSubobject<ULedgeDetectorComponent>(TEXT("Ledge Detector"));

	GetMesh()->CastShadow = true;
	GetMesh()->bCastDynamicShadow = true;

	CharacterAttributesComponent = CreateDefaultSubobject<UCharacterAttributeComponent>(TEXT("Attribute Component"));
	MotionWarpingComponent = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("MotionWarpingComp"));
	CharacterEquipmentComponent = CreateDefaultSubobject<UCharacterEquipmentComponent>(TEXT("Character Equipment Component"));

	LeftMeleeHitRegistrator = CreateDefaultSubobject<UMeleeHitRegistrator>(TEXT("LeftMeleeHitRegistrator"));
	RightMeleeHitRegistrator = CreateDefaultSubobject<UMeleeHitRegistrator>(TEXT("RightMeleeHitRegistrator"));

	LeftMeleeHitRegistrator->SetupAttachment(GetRootComponent());
	LeftMeleeHitRegistrator->SetHiddenInGame(false);
	LeftMeleeHitRegistrator->SetCollisionProfileName(NoCollisionProfile);
	LeftMeleeHitRegistrator->SetNotifyRigidBodyCollision(false);

	RightMeleeHitRegistrator->SetupAttachment(GetRootComponent());
	RightMeleeHitRegistrator->SetHiddenInGame(false);
	RightMeleeHitRegistrator->SetCollisionProfileName(NoCollisionProfile);
	RightMeleeHitRegistrator->SetNotifyRigidBodyCollision(false);

	PunchAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("PunchAudioComponent"));
	PunchAudioComponent->SetupAttachment(GetRootComponent());
}

void ABaseCharacter::OnLevelDeserialized_Implementation()
{
}

void ABaseCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	AAIController* AIController = Cast<AAIController>(NewController);

	if (IsValid(AIController))
	{
		FGenericTeamId TeamId((uint8)Team);
		AIController->SetGenericTeamId(TeamId);
	}
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

void ABaseCharacter::InteractWithRunWall()
{
	const ARunWall* AvailableRunWall = GetAvailableRunWall();
	if (IsValid(AvailableRunWall) && !GetBaseCharacterMovementComponent()->IsClimbing())
	{
		//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, TEXT("Wall Run Start"));
		GetBaseCharacterMovementComponent()->TryWallRun();
	}
}

const ARunWall* ABaseCharacter::GetAvailableRunWall() const
{
	const ARunWall* Result = nullptr;
	for (const AInteractiveActor* InteractiveActor : AvailableInteractiveActors)
	{
		if (InteractiveActor->IsA<ARunWall>())
		{
			Result = StaticCast<const ARunWall*>(InteractiveActor);
			break;
		}
	}
	return Result;
}

void ABaseCharacter::OnClimbActionStarted()
{
	if (!GetBaseCharacterMovementComponent()->IsClimbing() && !GetBaseCharacterMovementComponent()->IsWallRunning())
	{
		GetBaseCharacterMovementComponent()->ToggleClimbing(true);
		//JumpCount = 0;
	}
	/*else if(GetBaseCharacterMovementComponent()->IsClimbing())
	{
	}*/
}

void ABaseCharacter::InteractWithBeam()
{
	if (GetBaseCharacterMovementComponent()->IsOnBeam())
	{
		GetBaseCharacterMovementComponent()->StopWalkingOnBeam();
	}
	else
	{
		const ABeam* AvailableBeam = GetAvailableBeam();
		if (IsValid(AvailableBeam))
		{
			GetBaseCharacterMovementComponent()->StartWalkingOnBeam();
		}
	}
}

const ABeam* ABaseCharacter::GetAvailableBeam() const
{
	const ABeam* Result = nullptr;
	for (const AInteractiveActor* InteractiveActor : AvailableInteractiveActors)
	{
		if (InteractiveActor->IsA<ABeam>())
		{
			Result = StaticCast<const ABeam*>(InteractiveActor);
			break;
		}
	}
	return Result;
}

const UCharacterEquipmentComponent* ABaseCharacter::GetCharacterEquipmentComponent() const
{	
	return CharacterEquipmentComponent;
}

UCharacterEquipmentComponent* ABaseCharacter::GetCharacterEquipmentComponent_Mutable() const
{
	return CharacterEquipmentComponent;
}

void ABaseCharacter::Fire()
{
	if (CharacterEquipmentComponent->IsEquipping())
	{
		return;
	}
	ARangeWeaponItem* CurrentRangeWeapon = CharacterEquipmentComponent->GetCurrentRangeWeapon();
	if (IsValid(CurrentRangeWeapon))
	{
		CurrentRangeWeapon->StartFire();
	}
}

void ABaseCharacter::Reload() const
{
	if (IsValid(CharacterEquipmentComponent->GetCurrentRangeWeapon()))
	{
		CharacterEquipmentComponent->ReloadCurrentRangeWeapon();
	}
}

void ABaseCharacter::StartAiming()
{
	ARangeWeaponItem* CurrentRangeWeapon = GetCharacterEquipmentComponent()->GetCurrentRangeWeapon();
	if (!IsValid(CurrentRangeWeapon))
	{
		return;
	}

	bIsAiming = true;
	CurrentAimingMovementSpeed = CurrentRangeWeapon->GetAimMovementMaxSpeed();
	CurrentRangeWeapon->StartAim();
	OnStartAiming();
}

void ABaseCharacter::StopAiming()
{
	if (!bIsAiming)
	{
		return;
	}

	ARangeWeaponItem* CurrentRangeWeapon = GetCharacterEquipmentComponent()->GetCurrentRangeWeapon();
	if (IsValid(CurrentRangeWeapon))
	{
		CurrentRangeWeapon->StopAim();
	}

	bIsAiming = false;
	CurrentAimingMovementSpeed = 0.0f;
	OnStopAiming();
}

void ABaseCharacter::OnStartAiming_Implementation()
{	
	OnStartAimingIternal();
}

void ABaseCharacter::OnStopAiming_Implementation()
{	
	OnStopAimingIternal();
}

float ABaseCharacter::GetAimingMovementSpeed() const
{	
	return CurrentAimingMovementSpeed;
}

bool ABaseCharacter::IsAiming() const
{
	return bIsAiming;
}

void ABaseCharacter::NextItem()
{
	CharacterEquipmentComponent->EquipNextItem();
}

void ABaseCharacter::PreviousItem()
{
	CharacterEquipmentComponent->EquipPreviousItem();
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	CharacterAttributesComponent->OnDeathEvent.AddUObject(this, &ABaseCharacter::OnDeath);
	PunchAudioComponent->SetSound(PunchSoundBase);
}

bool ABaseCharacter::CanMantle() const
{
	return !GetBaseCharacterMovementComponent()->IsOnLadder();
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

void ABaseCharacter::OnStartAimingIternal()
{
}

void ABaseCharacter::OnStopAimingIternal()
{
}

void ABaseCharacter::EquipPrimaryItem()
{
	CharacterEquipmentComponent->EquipItemInSlot(EEquipmentSlots::PRIMARY_ITEM_SLOT);
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

void ABaseCharacter::MeleeAttackStart()
{
	LeftMeleeHitRegistrator->SetCollisionProfileName(MeleeCollisionProfileEnabled);
	LeftMeleeHitRegistrator->SetNotifyRigidBodyCollision(true);

	RightMeleeHitRegistrator->SetCollisionProfileName(MeleeCollisionProfileEnabled);
	RightMeleeHitRegistrator->SetNotifyRigidBodyCollision(true);
}

void ABaseCharacter::MeleeAttackFinish()
{
	LeftMeleeHitRegistrator->SetCollisionProfileName(NoCollisionProfile);
	LeftMeleeHitRegistrator->SetNotifyRigidBodyCollision(false);

	RightMeleeHitRegistrator->SetCollisionProfileName(NoCollisionProfile);
	RightMeleeHitRegistrator->SetNotifyRigidBodyCollision(false);
}

void ABaseCharacter::HandsMeleeAttack()
{
	AMeleeWeaponItem* CurrentMeleeWeaponItem = CharacterEquipmentComponent->GetCurrentMeleeWeaponItem();
	if (IsValid(CurrentMeleeWeaponItem))
	{	
		const FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, false);

		LeftMeleeHitRegistrator->AttachToComponent(GetMesh(), AttachmentRules, "hand_left_collision");
		RightMeleeHitRegistrator->AttachToComponent(GetMesh(), AttachmentRules, "hand_right_collision");

		CurrentMeleeWeaponItem->StartAttack(EMeleeAttackTypes::HANDS);

		PlayAudio(PunchAudioComponent);
	}
}

void ABaseCharacter::LegsMeleeAttack()
{
	AMeleeWeaponItem* CurrentMeleeWeaponItem = CharacterEquipmentComponent->GetCurrentMeleeWeaponItem();
	if (IsValid(CurrentMeleeWeaponItem))
	{	
		const FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, false);

		LeftMeleeHitRegistrator->AttachToComponent(GetMesh(), AttachmentRules, "foot_left_collision");
		RightMeleeHitRegistrator->AttachToComponent(GetMesh(), AttachmentRules, "foot_right_collision");

		CurrentMeleeWeaponItem->StartAttack(EMeleeAttackTypes::LEGS);

		PlayAudio(PunchAudioComponent);
	}
}

void ABaseCharacter::ThrowBomb()
{
	if (CharacterEquipmentComponent->IsEquipping())
	{
		return;
	}

	AThrowableItem* CurrentThrowableItem = CharacterEquipmentComponent->GetCurrentThrowableItem();

	if (IsValid(CurrentThrowableItem))
	{
		CurrentThrowableItem->StartThrow();
	}
}

void ABaseCharacter::PlayAudio(UAudioComponent* AudioComponent)
{
	if (AudioComponent && !AudioComponent->IsPlaying())
	{
		AudioComponent->Play(0.f);
	}
}

void ABaseCharacter::SetCanMove(bool value)
{
	bCanMove = value;
}

FGenericTeamId ABaseCharacter::GetGenericTeamId() const
{
	return FGenericTeamId((uint8)Team);
}
