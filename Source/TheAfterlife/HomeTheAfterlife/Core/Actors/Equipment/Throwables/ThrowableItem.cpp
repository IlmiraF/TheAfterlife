#include "ThrowableItem.h"
#include <TheAfterlife/HomeTheAfterlife/Core/Characters/BaseCharacter.h>
#include <TheAfterlife/HomeTheAfterlife/Core/Actors/Projectiles/Projectile.h>

void AThrowableItem::Throw()
{
	checkf(GetOwner()->IsA<ABaseCharacter>(), TEXT("AThrowableItem::Throw only character can be an owner of a throwable"));
	ABaseCharacter* CharacterOwner = StaticCast<ABaseCharacter*>(GetOwner());

	APlayerController* Controller = CharacterOwner->GetController<APlayerController>();
	if (!IsValid(Controller))
	{
		return;
	}

	FVector PlayerViewPoint;
	FRotator PlayerViewRotation;

	Controller->GetPlayerViewPoint(PlayerViewPoint, PlayerViewRotation);

	FVector ViewDirection = PlayerViewRotation.RotateVector(FVector::ForwardVector);

	FVector SpawnLocation = PlayerViewPoint + ViewDirection * 100.0f;

	AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, SpawnLocation, FRotator::ZeroRotator);

	if (IsValid(Projectile))
	{
		Projectile->SetOwner(GetOwner());
		Projectile->LaunchProjectile(ViewDirection);
	}
}
