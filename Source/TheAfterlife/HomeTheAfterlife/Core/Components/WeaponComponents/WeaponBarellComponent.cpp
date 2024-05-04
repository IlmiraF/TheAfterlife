// Fill out your copyright notice in the Description page of Project Settings.

#include "WeaponBarellComponent.h"
#include "../../../../TheAfterlifeTypes.h"
#include "DrawDebugHelpers.h"
#include "Engine/DamageEvents.h"
#include "../../Actors/Projectiles/Projectile.h"
#include "Kismet/GameplayStatics.h"

void UWeaponBarellComponent::Shot(FVector ShotDirection, float SpreadAngle)
{	

	for (int i = 0; i < BulletsPerShot; i++)
	{	
		FVector ShotStart = GetComponentLocation();

		ShotDirection += GetBulletSpreadOffset(FMath::RandRange(0.0f, SpreadAngle), ShotDirection.ToOrientationRotator());
		ShotDirection = ShotDirection.GetSafeNormal();

		FVector ShotEnd = ShotStart + FiringRange * ShotDirection;

		switch (HitRegistration)
		{
			case EHitRegistrationType::HitScan:
			{
				bool bHasHit = HitScan(ShotStart, ShotEnd, ShotDirection);
				break;
			}
			case EHitRegistrationType::Projectile:
			{
				LaunchProjectile(ShotStart, ShotDirection + FVector(0, 0, 0.1));
				break;
			}
		}

	}
}

void UWeaponBarellComponent::SetDamageMultiplier(float newDamageMultiplier)
{
	DamageMultiplier = newDamageMultiplier;
}

bool UWeaponBarellComponent::HitScan(FVector ShotStart, OUT FVector& ShotEnd, FVector ShotDirection)
{
	FHitResult ShotResult;
	bool bHasHit = GetWorld()->LineTraceSingleByChannel(ShotResult, ShotStart, ShotEnd, ECC_Bullet);
	if (bHasHit)
	{
		ShotEnd = ShotResult.ImpactPoint;
		ProcessHit(ShotResult, ShotDirection);
	}
	return bHasHit;
}

void UWeaponBarellComponent::LaunchProjectile(const FVector& LaunchStart, const FVector& LaunchDirection)
{
	AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, LaunchStart, LaunchDirection.ToOrientationRotator());
	if (IsValid(Projectile))
	{
		Projectile->SetOwner(GetOwningPawn());
		Projectile->OnProjectileHit.AddDynamic(this, &UWeaponBarellComponent::ProcessHit);
		Projectile->LaunchProjectile(LaunchDirection.GetSafeNormal());
	}
}

APawn* UWeaponBarellComponent::GetOwningPawn() const
{
	APawn* PawnOwner = Cast<APawn>(GetOwner());
	if (IsValid(PawnOwner))
	{
		PawnOwner = Cast<APawn>(GetOwner()->GetOwner());
	}
	return PawnOwner;
}

AController* UWeaponBarellComponent::GetController() const
{
	APawn* PawnOwner = GetOwningPawn();
	return IsValid(PawnOwner) ? PawnOwner->GetController() : nullptr;
}

void UWeaponBarellComponent::ProcessHit(const FHitResult& HitResult, const FVector& Direction)
{	
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("ProcessHit")));

	AActor* HitActor = HitResult.GetActor();
	if (IsValid(HitActor))
	{	
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("IsValid(HitActor)")));
		FString Name = HitActor->GetName();
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, Name);
		FPointDamageEvent DamageEvent;
		DamageEvent.HitInfo = HitResult;
		DamageEvent.ShotDirection = Direction;
		DamageEvent.DamageTypeClass = DamageTypeClass;
		HitActor->TakeDamage(DamageAmount * DamageMultiplier, DamageEvent, GetController(), GetOwner());
	}
}

FVector UWeaponBarellComponent::GetBulletSpreadOffset(float Angle, FRotator ShotRotation) const
{
	float SpreadSize = FMath::Tan(Angle);
	float RotationAngle = FMath::RandRange(0.0f, 2 * PI);

	float SpreadY = FMath::Cos(RotationAngle);
	float SpreadZ = FMath::Sin(RotationAngle);

	FVector Result = (ShotRotation.RotateVector(FVector::UpVector) * SpreadZ
		+ ShotRotation.RotateVector(FVector::RightVector) * SpreadY) * SpreadSize;

	return Result;
}