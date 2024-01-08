// Fill out your copyright notice in the Description page of Project Settings.


#include "MovingPlatform.h"
#include "Components/StaticMeshComponent.h"
#include "PlatformTrigger.h"

void AMovingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	PlatformTimeline.TickTimeline(DeltaTime);
}

void AMovingPlatform::MovePlatform()
{
	if (PlatformTimeline.IsPlaying())
	{
		return;
	}

	if (bIsMovingForward)
	{
		PlatformTimeline.Reverse();
		bIsMovingForward = false;
	}
	else
	{
		if (bIsForwardBackward)
		{
			EndLocation = GetActorTransform().TransformPosition(FirstEndPlatformLocation);
		}
		else
		{
			EndLocation = GetActorTransform().TransformPosition(SecondEndPlatformLocation);
		}
		PlatformTimeline.Play();
		bIsMovingForward = true;
	}
	GetWorld()->GetTimerManager().ClearTimer(ReturnTimer);

}

void AMovingPlatform::BeginPlay()
{
	Super::BeginPlay();
	SetActorTickEnabled(false);
	if (IsValid(MovementCurve))
	{
		FOnTimelineFloatStatic OnTimelineUpdate;
		OnTimelineUpdate.BindUFunction(this, FName("TickPlatformTimeline"));
		PlatformTimeline.AddInterpFloat(MovementCurve, OnTimelineUpdate);

		FOnTimelineEventStatic OnTimelineFinished;
		OnTimelineFinished.BindUFunction(this, FName("OnPlatfromEndReached"));
		PlatformTimeline.SetTimelineFinishedFunc(OnTimelineFinished);
	}
	
	StartLocation = GetActorLocation();

	if (IsValid(PlatformTrigger))
	{
		PlatformTrigger->OnTriggerActivated.AddDynamic(this, &AMovingPlatform::OnPlatformTriggerActivated);
	}
}

void AMovingPlatform::OnPlatformTriggerActivated(bool bIsActivated)
{
	bIsForwardBackward = !bIsForwardBackward;
	if(bIsFirstTime)
	{
		SetActorTickEnabled(true);
		bIsFirstTime = false;
		MovePlatform();
	}
}

void AMovingPlatform::TickPlatformTimeline(float Value)
{
	FVector NewLocation = FMath::Lerp(StartLocation, EndLocation, Value);
	SetActorLocation(NewLocation);
}

void AMovingPlatform::OnPlatfromEndReached()
{
	if (ReturnTime > 0.0f)
	{
		GetWorld()->GetTimerManager().SetTimer(ReturnTimer, this, &AMovingPlatform::MovePlatform, ReturnTime, false);
		return;
	}
}