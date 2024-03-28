// Fill out your copyright notice in the Description page of Project Settings.


#include "Bird.h"
#include "../../Actors/Interactive/Environment/TutorialCollider.h"

ABird::ABird()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ABird::BeginPlay()
{
	Super::BeginPlay();
	for (ATutorialCollider* Collider : TutorialColliderArray)
	{
		Collider->OnReachedTargetEvent.AddUObject(this, &ABird::SetNewPoint);
	}
}

void ABird::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Fly(DeltaTime);

}

void ABird::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ABird::SetNewPoint()
{
	if (CurrnetIndex < (RouteArray.Num() - 1))
	{
		CurrnetIndex = CurrnetIndex + 1;
	}
}

void ABird::Fly(float DeltaTime)
{
	FVector StartLocation = GetActorLocation();
	FVector EndLocation = RouteArray[CurrnetIndex];

	float Distance = FVector::Distance(StartLocation, EndLocation);

	if (Distance < DeltaPosition)
	{
		return;
	}

	FVector Direction = (EndLocation - StartLocation).GetSafeNormal();
	FRotator Rotation = Direction.Rotation();
	FQuat RotationQuat = Rotation.Quaternion();

	FVector CurrentLocation = GetActorLocation();
	CurrentLocation += Direction * Speed * DeltaTime;

	float SinusoidOffset = GetSinusoidOffset(DeltaTime, SinusoidHeight, SinusoidFrequency);
	CurrentLocation.Z += SinusoidOffset;

	SetActorLocationAndRotation(CurrentLocation, RotationQuat, false, 0, ETeleportType::None);
}

float ABird::GetSinusoidOffset(float DeltaTime, float Height, float Frequency)
{
	CurrentTime += DeltaTime;

	float Angle = CurrentTime * Frequency * 2 * PI;

	float Offset = Height * FMath::Sin(Angle);

	return Offset;
}