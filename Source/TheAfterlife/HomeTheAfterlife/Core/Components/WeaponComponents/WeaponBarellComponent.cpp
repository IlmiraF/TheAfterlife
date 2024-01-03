// Fill out your copyright notice in the Description page of Project Settings.

#include "../Source\TheAfterlife\TheAfterlifeTypes.h"
#include "DrawDebugHelpers.h"
#include "WeaponBarellComponent.h"


void UWeaponBarellComponent::Shot()
{
	FVector ShotStart = GetComponentLocation();
	FVector ShotDirection = GetComponentRotation().RotateVector(FVector::ForwardVector);
	FVector ShotEnd = ShotStart + FiringRange * ShotDirection;

	FHitResult ShotResult;
	if (GetWorld()->LineTraceSingleByChannel(ShotResult, ShotStart, ShotEnd, ECC_Visibility))
	{	
		ShotEnd = ShotResult.ImpactPoint;
		DrawDebugSphere(GetWorld(), ShotEnd, 10.0f, 24, FColor::Red, false, 1.0f);
		GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Green, FString::Printf(TEXT("YGA BUGA")));
	}
	DrawDebugLine(GetWorld(), ShotStart, ShotEnd, FColor::Green, false, 1.0f, 0, 3.0f);
	GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Blue, FString::Printf(TEXT("YGA BUGA")));
}
