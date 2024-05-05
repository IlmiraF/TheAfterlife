#include "RangeWeaponItem.h"
#include "../../../Components/WeaponComponents/WeaponBarellComponent.h"
#include "../../../Characters/BaseCharacter.h"


ARangeWeaponItem::ARangeWeaponItem()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("WeaponRoot"));

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(RootComponent);

	WeaponBarell = CreateDefaultSubobject<UWeaponBarellComponent>(TEXT("WeaponBarell"));
	WeaponBarell->SetupAttachment(WeaponMesh, "MuzzleSocket");

	ReticleType = EReticleType::Default;

	EquippedSocketName = SocketCharacterWeapon;
}

void ARangeWeaponItem::StartFire()
{
	if (GetWorld()->GetTimerManager().IsTimerActive(ShotTimer))
	{
		return;
	}

	bIsFiring = true;
	MakeShot();
}

void ARangeWeaponItem::StopFire()
{	
	checkf(GetOwner()->IsA<ABaseCharacter>(), TEXT("ARangeWeaponItem::MakeShot() only character can be an owner of range weapon"));
	ABaseCharacter* CharacterOwner = GetCharacterOwner();

	CharacterOwner->StopAnimMontage(CharacterFireMontage);
	CharacterOwner->StopAnimMontage(WeaponFireMontage);
	bIsFiring = false;
}

void ARangeWeaponItem::MakeShot()
{	
	ABaseCharacter* CharacterOwner = GetCharacterOwner();
	if (!IsValid(CharacterOwner))
	{
		return;
	}

	if (!CanShoot())
	{
		StopFire();
		if (Ammo == 0 && bAutoReload)
		{
			CharacterOwner->Reload();
		}
		return;
	}

	EndReload(false);
	CharacterOwner->PlayAnimMontage(CharacterFireMontage);
	PlayAnimMontage(WeaponFireMontage);

	SetAmmo(Ammo - 1);

	FVector ShotLocation;
	FRotator ShotRotation;
	
	if (CharacterOwner->IsPlayerControlled())
	{
		APlayerController* Controller = CharacterOwner->GetController<APlayerController>();
		Controller->GetPlayerViewPoint(ShotLocation, ShotRotation);
	}
	else
	{
		ShotLocation = WeaponBarell->GetComponentLocation();
		ShotRotation = CharacterOwner->GetBaseAimRotation();
	}
	
	
	FVector ShotDirection = ShotRotation.RotateVector(FVector::ForwardVector);

	WeaponBarell->Shot(ShotDirection, GetCurrentBulletSpreadAngle());

	GetWorld()->GetTimerManager().SetTimer(ShotTimer, this, &ARangeWeaponItem::OnShotTimerElapsed, GetShotTimerInterval(), false);
}

bool ARangeWeaponItem::IsFiring() const
{
	return bIsFiring;
}

void ARangeWeaponItem::StartAim()
{
	bIsAiming = true;
}

void ARangeWeaponItem::StopAim()
{
	bIsAiming = false;
}

void ARangeWeaponItem::StartReload()
{

}

void ARangeWeaponItem::EndReload(bool bIsSuccess)
{
	
}

int32 ARangeWeaponItem::GetAmmo() const
{
	return Ammo;
}

int32 ARangeWeaponItem::GetMaxAmmo() const
{
	return MaxAmmo;
}

void ARangeWeaponItem::SetAmmo(int32 NewAmmo)
{
	Ammo = NewAmmo;
	if (OnAmmoChanged.IsBound())
	{
		OnAmmoChanged.Broadcast(Ammo);
	}
}

EAmunitionType ARangeWeaponItem::GetAmmoType() const
{
	return AmmoType;
}

bool ARangeWeaponItem::CanShoot() const
{
	return Ammo > 0;
}

float ARangeWeaponItem::GetAimFOV() const
{
	return AimFOV;
}

float ARangeWeaponItem::GetAimMovementMaxSpeed() const
{
	return AimMovementMaxSpeed;
}

FTransform ARangeWeaponItem::GetForeGripTransform() const
{
	return WeaponMesh->GetSocketTransform("ForeGripSocket");
}

EReticleType ARangeWeaponItem::GetReticleType() const
{
	return bIsAiming ? AimReticleType : ReticleType;
}

void ARangeWeaponItem::SetWeaponBooster(float newDamageMultiplier, float newAccuracyMultiplier)
{	
	GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Cyan, FString::Printf(TEXT("Famage: %f"), newDamageMultiplier));
	AccuracyMultiplier = newAccuracyMultiplier;
	WeaponBarell->SetDamageMultiplier(newDamageMultiplier);
}

void ARangeWeaponItem::SetDefaultWeaponBooster()
{
	AccuracyMultiplier = 1;
	WeaponBarell->SetDamageMultiplier(1);
}

void ARangeWeaponItem::BeginPlay()
{
	Super::BeginPlay();
	SetAmmo(MaxAmmo);
}

float ARangeWeaponItem::GetCurrentBulletSpreadAngle() const
{
	float AngleInDegrees = bIsAiming ? AimSpreadAngle : SpreadAngle;
	return FMath::DegreesToRadians(AngleInDegrees*AccuracyMultiplier);
}

void ARangeWeaponItem::MakeShotAnim()
{
	checkf(GetOwner()->IsA<ABaseCharacter>(), TEXT("ARangeWeaponItem::MakeShotAnim() only character can be an owner of range weapon"));
	ABaseCharacter* CharacterOwner = GetCharacterOwner();

	if (!CanShoot())
	{
		StopFire();
		if (Ammo == 0 && bAutoReload)
		{
			CharacterOwner->Reload();
		}
		return;
	}

	EndReload(false);
	CharacterOwner->PlayAnimMontage(CharacterFireMontage);
	PlayAnimMontage(WeaponFireMontage);
}

void ARangeWeaponItem::OnShotTimerElapsed()
{
	if (!bIsFiring)
	{
		return;
	}

	switch (WeaponFireMode)
	{
		case EWeaponFireMode::Single:
		{
			StopFire();
			break;
		}
		case EWeaponFireMode::FullAuto:
		{
			MakeShotAnim();
			break;
		}
	}
}

float ARangeWeaponItem::GetShotTimerInterval() const
{
	return 60.0f / RateOfFire;
}

float ARangeWeaponItem::PlayAnimMontage(UAnimMontage* AnimMontage)
{
	UAnimInstance* WeaponAnimInstance = WeaponMesh->GetAnimInstance();
	float Result = 0.0f;
	if (IsValid(WeaponAnimInstance))
	{
		Result = WeaponAnimInstance->Montage_Play(AnimMontage);
	}
	return Result;
}

void ARangeWeaponItem::StopAnimMontage(UAnimMontage* AnimMontage, float BlendOutTime /*= 0.0f*/)
{
	UAnimInstance* WeaponAnimInstance = WeaponMesh->GetAnimInstance();
	if (IsValid(WeaponAnimInstance))
	{
		WeaponAnimInstance->Montage_Stop(BlendOutTime, AnimMontage);
	}
}
