// Fill out your copyright notice in the Description page of Project Settings.

#include "ExplosiveProjectile.h"
#include "../../Components\WeaponComponents\ExplosiveComponent.h"

AExplosiveProjectile::AExplosiveProjectile()
{
	ExplosiveComponent = CreateDefaultSubobject<UExplosiveComponent>(TEXT("ExplosionComponent"));
	ExplosiveComponent->SetupAttachment(GetRootComponent());

}

void AExplosiveProjectile::OnProjectileLaunched()
{
	Super::OnProjectileLaunched();

	GetWorld()->GetTimerManager().SetTimer(DetonationTimer, this, &AExplosiveProjectile::OnDetonationTimerElapsed, DetonationTime, false);
}

AController* AExplosiveProjectile::GetContrller()
{
	APawn* PawnOwner = Cast<APawn>(GetOwner());

	return IsValid(PawnOwner) ? PawnOwner->GetController() : nullptr;
}

void AExplosiveProjectile::OnDetonationTimerElapsed()
{
	ExplosiveComponent->Explode(GetContrller());
}
