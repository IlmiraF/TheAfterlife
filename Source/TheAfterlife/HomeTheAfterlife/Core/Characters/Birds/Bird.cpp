// Fill out your copyright notice in the Description page of Project Settings.


#include "Bird.h"
#include "Components/AudioComponent.h"
#include "AIController.h"

ABird::ABird()
{
	PrimaryActorTick.bCanEverTick = true;

	SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComponent"));
	SplineComponent->SetupAttachment(GetRootComponent());

	BirdMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	BirdMesh->SetupAttachment(GetRootComponent());

	BirdAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	BirdAudioComponent->SetupAttachment(BirdMesh);
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
		float InputKey = SplineComponent->FindInputKeyClosestToWorldLocation(Point);
		StopDistances.Add(SplineComponent->GetDistanceAlongSplineAtSplineInputKey(InputKey));
	}
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

void ABird::Speak(USoundBase* SoundBase)
{
	BirdAudioComponent->SetSound(SoundBase);
	if (BirdAudioComponent && !BirdAudioComponent->IsPlaying())
	{
		BirdAudioComponent->Play(0.f);
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