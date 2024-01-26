// Fill out your copyright notice in the Description page of Project Settings.

#include "TheAfterlife\HomeTheAfterlife\Core\Utils\TheAfterlife_TraceUtils.h"
#include "../Source\TheAfterlife\TheAfterlifeTypes.h"
#include "MeleeHitRegistrator.h"

UMeleeHitRegistrator::UMeleeHitRegistrator()
{
	PrimaryComponentTick.bCanEverTick = true;
	SphereRadius = 10.0f;
	SetCollisionProfileName("Weapon");
}

void UMeleeHitRegistrator::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bIsHitRegistrationEnabled)
	{
		ProcessHitRegistration();
	}
	PreviousComponentLocation = GetComponentLocation();
}

void UMeleeHitRegistrator::ProcessHitRegistration()
{
	FVector CurrentLocation = GetComponentLocation();

	FHitResult HitResult;

	bool bHasHit = TheAfterlife_TraceUtils::SweepSphereSingleByChannel(
		GetWorld(),
		HitResult,
		PreviousComponentLocation,
		CurrentLocation,
		GetScaledSphereRadius(),
		ECC_Melee,
		FCollisionQueryParams::DefaultQueryParam,
		FCollisionResponseParams::DefaultResponseParam,
		false,
		5.0f
	);

	if (bHasHit)
	{	
		if (HitResult.GetActor() == GetOwner())
		{	
			return;
		}

		FVector Direction = (CurrentLocation - PreviousComponentLocation).GetSafeNormal();
		if (OnMeleeHitRegistred.IsBound())
		{
			OnMeleeHitRegistred.Broadcast(HitResult, Direction);
		}
	}
}

void UMeleeHitRegistrator::SetIsHitRegistrationEnabled(bool bIsEnabled_In)
{
	bIsHitRegistrationEnabled = bIsEnabled_In;
}