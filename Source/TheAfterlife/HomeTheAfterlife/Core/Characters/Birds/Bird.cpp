// Fill out your copyright notice in the Description page of Project Settings.


#include "Bird.h"
#include "AIController.h"

ABird::ABird()
{
	PrimaryActorTick.bCanEverTick = true;

	SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComponent"));
	SplineComponent->SetupAttachment(GetRootComponent());

	BirdMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	BirdMesh->SetupAttachment(GetRootComponent());
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
		//SplineComponent->AddSplinePoint(Point, ESplineCoordinateSpace::World, true);
		float InputKey = SplineComponent->FindInputKeyClosestToWorldLocation(Point);
		StopDistances.Add(SplineComponent->GetDistanceAlongSplineAtSplineInputKey(InputKey));
	}

	//SplineComponent->SetClosedLoop(false);
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
	
	if (DistanceAlongSpline >= StopDistances[CurrentIndex])
	{
		return;
	}

	DistanceAlongSpline += Speed * DeltaTime;

	FVector CurrentSplineLocation = SplineComponent->GetLocationAtDistanceAlongSpline(DistanceAlongSpline, ESplineCoordinateSpace::World);
	FRotator CurrentSplineRotation = SplineComponent->GetRotationAtDistanceAlongSpline(DistanceAlongSpline, ESplineCoordinateSpace::World);
	CurrentSplineRotation = CurrentSplineRotation - FRotator(0.f, 90.0f, 0.f);

	BirdMesh->SetWorldLocationAndRotation(CurrentSplineLocation, CurrentSplineRotation);

}

float ABird::GetSinusoidOffset(float DeltaTime, float Height, float Frequency)
{
	CurrentTime += DeltaTime;

	float Angle = CurrentTime * Frequency * 2 * PI;

	float Offset = Height * FMath::Sin(Angle);

	return Offset;
}