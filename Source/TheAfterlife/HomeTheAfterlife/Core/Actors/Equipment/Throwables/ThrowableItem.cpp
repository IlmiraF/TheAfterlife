#include "ThrowableItem.h"
#include <TheAfterlife/HomeTheAfterlife/Core/Characters/BaseCharacter.h>
#include <TheAfterlife/HomeTheAfterlife/Core/Actors/Projectiles/Projectile.h>

void AThrowableItem::Throw()
{	
	
	ABaseCharacter* CharacterOwner = GetCharacterOwner();
	if (!IsValid(CharacterOwner))
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
	FTransform PlayerViewTransform(PlayerViewRotation, PlayerViewPoint);

	Controller->GetPlayerViewPoint(PlayerViewPoint, PlayerViewRotation);

	FVector ViewDirection = PlayerViewRotation.RotateVector(FVector::ForwardVector);
	FVector ViewUpVector = PlayerViewRotation.RotateVector(FVector::UpVector);
	
	FVector LaunchDirection = ViewDirection + FMath::Tan(FMath::DegreesToRadians(ThrowAngle)) * ViewUpVector;

	FVector ThrowableSocketLoaction = CharacterOwner->GetMesh()->GetSocketLocation("hand_right_collision");
	FVector SocketInViewSpace = PlayerViewTransform.InverseTransformPosition(ThrowableSocketLoaction);

	FVector SpawnLocation = PlayerViewPoint + ViewDirection * SocketInViewSpace.X;

	AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, SpawnLocation, FRotator::ZeroRotator);

	if (IsValid(Projectile))
	{
		Projectile->SetOwner(GetOwner());
		Projectile->LaunchProjectile(LaunchDirection.GetSafeNormal());
	}
}
