// Fill out your copyright notice in the Description page of Project Settings.


#include "RotatingPlatform.h"
#include "../../Characters/PlayerCharacter.h"

void ARotatingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	PlatformTimeline.TickTimeline(DeltaTime);
}

void ARotatingPlatform::BeginPlay()
{
	Super::BeginPlay();
	if (IsValid(MovementCurve))
	{
		FOnTimelineFloatStatic OnTimelineUpdate;
		OnTimelineUpdate.BindUFunction(this, FName("TickPlatformTimeline"));
		PlatformTimeline.AddInterpFloat(MovementCurve, OnTimelineUpdate);

		//FOnTimelineEventStatic OnTimelineFinished;
		//OnTimelineFinished.BindUFunction(this, FName("OnPlatfromEndReached"));
		//PlatformTimeline.SetTimelineFinishedFunc(OnTimelineFinished);
	}
}

void ARotatingPlatform::OnPlatformTriggerActivated(bool bIsActivated)
{
	Super::OnPlatformTriggerActivated(bIsActivated);
	bIsReached = !bIsReached;
	if (bIsReached)
	{
		PlatformTimeline.Play();
	}
	else
	{
		PlatformTimeline.Reverse();
	}
}

void ARotatingPlatform::TickPlatformTimeline(float Value)
{
	float NewValue = FMath::Lerp(First, Second, Value);
	FRotator NewRotation = GetActorRotation();
	NewRotation.Pitch = NewValue;
	SetActorRotation(NewRotation);
}

//void ARotatingPlatform::OnPlatfromEndReached()
//{
//	SetActorTickEnabled(false);
//}