#include "ThrowableItem.h"
#include "TheAfterlife/HomeTheAfterlife/Core/Characters/BaseCharacter.h"
#include "TheAfterlife/HomeTheAfterlife/Core/Actors/Projectiles/Projectile.h"

void AThrowableItem::StartThrow()
{
	if (CanThrow() == false)
	{
		return;
	}

	ABaseCharacter* CharacterOwner = GetCharacterOwner();

	CharacterOwner->PlayAnimMontage(ThrowItemMontage);
}

void AThrowableItem::Throw()
{	
	ABaseCharacter* CharacterOwner = GetCharacterOwner();
	if (!IsValid(CharacterOwner))
	{
		return;
	}

	if (!CanThrow())
	{
		return;
	}

	APlayerController* Controller = CharacterOwner->GetController<APlayerController>();
	if (!IsValid(Controller))
	{
		return;
	}

	FVector PlayerViewPoint;
	FRotator PlayerViewRotation;

	Controller->GetPlayerViewPoint(PlayerViewPoint, PlayerViewRotation);

	FVector ViewDirection = PlayerViewRotation.RotateVector(FVector::ForwardVector);
	FVector ViewUpVector = PlayerViewRotation.RotateVector(FVector::UpVector);
	
	FVector LaunchDirection = ViewDirection + FMath::Tan(FMath::DegreesToRadians(ThrowAngle)) * ViewUpVector;

	FVector ThrowableSocketLocaction = CharacterOwner->GetMesh()->GetSocketLocation("hand_right_collision");

	AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, ThrowableSocketLocaction, FRotator::ZeroRotator);

	if (IsValid(Projectile))
	{
		Projectile->SetOwner(GetOwner());
		Projectile->LaunchProjectile(LaunchDirection.GetSafeNormal());
	}

	SetAmmo(GetAmmo() - 1);
}

int32 AThrowableItem::GetAmmo() const
{
	return CurrentAmmo;
}

void AThrowableItem::SetAmmo(int32 NewAmmo)
{
	CurrentAmmo = NewAmmo;

	if (OnAmmoChanged.IsBound())
	{	
		OnAmmoChanged.Broadcast(CurrentAmmo);
	}
}

bool AThrowableItem::CanThrow()
{
	return CurrentAmmo > 0;
}