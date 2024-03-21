// Fill out your copyright notice in the Description page of Project Settings.


#include "Bird.h"
#include "AIController.h"

ABird::ABird()
{
	PrimaryActorTick.bCanEverTick = true;

	SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComponent"));
	SplineComponent->SetupAttachment(GetRootComponent());
}

void ABird::BeginPlay()
{
	Super::BeginPlay();

	InitializeSpline();
}

void ABird::InitializeSpline()
{
	for (const FVector& Point : RouteArray)
	{
		SplineComponent->AddSplinePoint(Point, ESplineCoordinateSpace::World, true);
		float InputKey = SplineComponent->FindInputKeyClosestToWorldLocation(Point);
		StopDistances.Add(SplineComponent->GetDistanceAlongSplineAtSplineInputKey(InputKey));
	}

	SplineComponent->SetClosedLoop(false);
}


void ABird::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Fly(DeltaTime);
}

void ABird::SetNewPoint(int32 Index)
{
	if (Index > CurrentIndex)
	{
		CurrentIndex = Index;
	}
}

void ABird::Fly(float DeltaTime)
{
	if (!SplineComponent || !RouteArray.IsValidIndex(CurrentIndex))
	{
		return;
	}

	float InputKey = SplineComponent->FindInputKeyClosestToWorldLocation(GetActorLocation());
	float CurrentDistance = SplineComponent->GetDistanceAlongSplineAtSplineInputKey(InputKey);

	if (CurrentDistance >= StopDistances[CurrentIndex])
	{
		return;
	}

	const FVector Direction = SplineComponent->GetDirectionAtSplineInputKey(InputKey, ESplineCoordinateSpace::World);
	FRotator Rotation = Direction.Rotation();
	FQuat RotationQuat = Rotation.Quaternion();

	FVector CurrentLocation = GetActorLocation();
	CurrentLocation += Direction * Speed * DeltaTime;

	float SinusoidOffset = GetSinusoidOffset(DeltaTime, SinusoidHeight, SinusoidFrequency);
	CurrentLocation.Z += SinusoidOffset;

	SetActorLocationAndRotation(CurrentLocation, RotationQuat);

}

float ABird::GetSinusoidOffset(float DeltaTime, float Height, float Frequency)
{
	CurrentIndex += DeltaTime;

	float Angle = CurrentIndex * Frequency * 2 * PI;

	float Offset = Height * FMath::Sin(Angle);

	return Offset;
}