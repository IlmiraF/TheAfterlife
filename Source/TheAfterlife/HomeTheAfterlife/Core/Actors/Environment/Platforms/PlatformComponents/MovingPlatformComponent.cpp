// Fill out your copyright notice in the Description page of Project Settings.


#include "MovingPlatformComponent.h"
#include "../PlatformBase.h"
#include "../PlatformTrigger.h"

UMovingPlatformComponent::UMovingPlatformComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UMovingPlatformComponent::BeginPlay()
{
	Super::BeginPlay();

	CachedPlatformBase = StaticCast<APlatformBase*>(GetOwner());

	if (IsValid(MovementCurve))
	{
		FOnTimelineFloatStatic OnTimelineUpdate;
		OnTimelineUpdate.BindUFunction(this, FName("TickPlatformTimeline"));
		PlatformTimeline.AddInterpFloat(MovementCurve, OnTimelineUpdate);

		FOnTimelineEventStatic OnTimelineFinished;
		OnTimelineFinished.BindUFunction(this, FName("OnPlatfromEndReached"));
		PlatformTimeline.SetTimelineFinishedFunc(OnTimelineFinished);
	}

	StartLocation = CachedPlatformBase->GetActorLocation();

	if (IsValid(PlatformTrigger))
	{
		PlatformTrigger->OnTriggerActivated.AddDynamic(this, &UMovingPlatformComponent::OnPlatformTriggerActivated);
	}
}

void UMovingPlatformComponent::OnPlatformTriggerActivated(bool bIsActivated)
{
	bIsForwardBackward = !bIsForwardBackward;
	if (bIsFirstTime)
	{
		bIsFirstTime = false;
		MovePlatform();
	}
}

void UMovingPlatformComponent::TickPlatformTimeline(float Value)
{
	FVector NewLocation = FMath::Lerp(StartLocation, EndLocation, Value);
	CachedPlatformBase->SetActorLocation(NewLocation);
}

void UMovingPlatformComponent::OnPlatfromEndReached()
{
	if (ReturnTime > 0.0f)
	{
		GetWorld()->GetTimerManager().SetTimer(ReturnTimer, this, &UMovingPlatformComponent::MovePlatform, ReturnTime, false);
		return;
	}
}

void UMovingPlatformComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	PlatformTimeline.TickTimeline(DeltaTime);
}

void UMovingPlatformComponent::MovePlatform()
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
			EndLocation = CachedPlatformBase->GetActorLocation() + FirstEndPlatformLocation;
		}
		else
		{
			EndLocation = CachedPlatformBase->GetActorLocation() + SecondEndPlatformLocation;
		}
		PlatformTimeline.Play();
		bIsMovingForward = true;
	}
	GetWorld()->GetTimerManager().ClearTimer(ReturnTimer);
}

