// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/AudioComponent.h"
#include "Engine/DataTable.h"
#include "AIController.h"
#include "TheAfterlife\TheAfterlifeTypes.h"
#include "BaseCharacter.generated.h"


USTRUCT(BlueprintType)
struct FPlayerAttackMontage : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UAnimMontage* Montage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 AnimSectionCount;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString Description;
};

USTRUCT(BlueprintType)
struct FMeleeCollisionProfile
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Enabled;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Disabled;

	FMeleeCollisionProfile()
	{
		Enabled = FName(TEXT("Weapon"));
		Disabled = FName(TEXT("NoCollision"));
	}
};

USTRUCT(BlueprintType)
struct FMantlingSettings
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UAnimMontage* MantlingMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UCurveVector* MantlingCurve;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = 0.0f, UIMin = 0.0f))
	float AnimationCorrectionXY = 65.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = 0.0f, UIMin = 0.0f))
	float AnimationCorrectionZ = 200.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = 0.0f, UIMin = 0.0f))
	float MaxHeight = 200.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = 0.0f, UIMin = 0.0f))
	float MinHeight = 100.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = 0.0f, UIMin = 0.0f))
	float MaxHeightStartTime = 0.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = 0.0f, UIMin = 0.0f))
	float MinHeightStartTime = 0.5f;
};

class UBaseCharacterMovementComponent;
class UCharacterAttributeComponent;
class AInteractiveActor;
class UMeleeCombatComponent;
class UCharacterEquipmentComponent;

typedef TArray<AInteractiveActor*, TInlineAllocator<10>> TInteractiveActorsArray;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnAimingStateChanged, bool)

UCLASS()
class THEAFTERLIFE_API ABaseCharacter : public ACharacter, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	ABaseCharacter(const FObjectInitializer& ObjectInitializer);

	virtual void PossessedBy(AController* NewController) override;

	FORCEINLINE UBaseCharacterMovementComponent* GetBaseCharacterMovementComponent() const { return BaseCharacterMovementComponent; }
	FORCEINLINE UCharacterAttributeComponent* GetCharacterAttributeComponent() const { return CharacterAttributesComponent; };

	virtual void MoveForward(float value) {};
	virtual void MoveRight(float value) {};
	virtual void Turn(float value) {};
	virtual void LookUp(float value) {};
	virtual void Jump();
	virtual void ChangeCrouchState();
	virtual void Falling() override;
	virtual void NotifyJumpApex() override;
	virtual void Landed(const FHitResult& Hit) override;

	UFUNCTION(BlueprintCallable)
	void Mantle(bool bForce = false);
	bool bIsMantling;

	void RegisterInteractiveActor(AInteractiveActor* InteractiveActor);
	void UnregisterInteractiveActor(AInteractiveActor* InteractiveActor);

	void ClimbLadderUp(float Value);
	void InteractWithLadder();
	const class ALadder* GetAvailableLadder() const;

	void InteractWithZipline();
	const class AZipline* GetAvailableZipline() const;

	const UCharacterEquipmentComponent* GetCharacterEquipmentComponent() const;

	UCharacterEquipmentComponent* GetCharacterEquipmentComponent_Mutable() const;

	FOnAimingStateChanged OnAimingStateChanged; 

	void Fire();

	void Reload() const;

	void StartAiming();
	void StopAiming();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Character")
	void OnStartAiming();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Character")
	void OnStopAiming();

	float GetAimingMovementSpeed() const;

	bool IsAiming() const;

	void NextItem();
	void PreviousItem();

	void EquipPrimaryItem();

	void MeleeAttackStart();
	void MeleeAttackFinish();

	void HandsMeleeAttack();
	void LegsMeleeAttack();

	void ThrowBomb();

	UFUNCTION(BlueprintCallable)
	void SetCanMove(bool Enabled);

	virtual FGenericTeamId GetGenericTeamId() const override;

protected:
	virtual void BeginPlay() override;

	UBaseCharacterMovementComponent* BaseCharacterMovementComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character|Movement")
	class ULedgeDetectorComponent* LedgeDetectorComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character|Movement|Jumping")
	int32 JumpCount = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character|Components")
	UCharacterAttributeComponent* CharacterAttributesComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character|Movement|Mantling")
	FMantlingSettings HighMantleSettings;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character|Movement|Mantling")
	FMantlingSettings LowMantleSettings;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character|Movement|Mantling", meta = (ClampMin = 0.0f, UIMin = 0.0f))
	float LowMantleMaxHeight = 125.0f;

	bool CanMantle() const;

	virtual void OnDeath();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character|Animations")
	class UAnimMontage* OnDeathAnimMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character|Animations")
	class UAnimMontage* MeleeCombatMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character|Attributes")
	class UCurveFloat* FallDamageCurve;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character|Components")
	class UCharacterEquipmentComponent* CharacterEquipmentComponent;

	virtual void OnStartAimingIternal();

	virtual void OnStopAimingIternal();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character|Melee|Collisions")
	class UMeleeHitRegistrator* LeftMeleeHitRegistrator;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character|Melee|Collisions")
	class UMeleeHitRegistrator* RightMeleeHitRegistrator;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character|Audio")
	class USoundBase* PunchSoundBase;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character|Animations")
	class UDataTable* PlayerAttackDataTable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character|Animations")
	float AnimationVariable;

	bool bCanMove = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character | Team")
	ETeams Team = ETeams::ENEMY;

private:
	const FMantlingSettings& GetMantlingSettings(float LedgeHeight) const;

	TInteractiveActorsArray AvailableInteractiveActors;

	void EnableRagdoll();
	FVector CurrentFallApex;

	UAudioComponent* PunchAudioComponent;

	FPlayerAttackMontage* AttackMontage;

	bool IsAnimationBlended;

	FMeleeCollisionProfile MeleeCollisionProfile;

	void PlayAudio(UAudioComponent* AudioComponent);

	bool bIsAiming;
	float CurrentAimingMovementSpeed;
};
