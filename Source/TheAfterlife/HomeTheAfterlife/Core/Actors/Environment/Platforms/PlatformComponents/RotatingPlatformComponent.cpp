// Fill out your copyright notice in the Description page of Project Settings.


#include "RotatingPlatformComponent.h"
#include "../PlatformTrigger.h"
#include "../PlatformBase.h"

URotatingPlatformComponent::URotatingPlatformComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void URotatingPlatformComponent::BeginPlay()
{
	Super::BeginPlay();

	CachedPlatformBase = StaticCast<APlatformBase*>(GetOwner());

	if (IsValid(MovementCurve))
	{
		FOnTimelineFloatStatic OnTimelineUpdate;
		OnTimelineUpdate.BindUFunction(this, FName("TickPlatformTimeline"));
		PlatformTimeline.AddInterpFloat(MovementCurve, OnTimelineUpdate);
	}

	PlatformTrigger->OnTriggerActivated.AddDynamic(this, &URotatingPlatformComponent::OnPlatformTriggerActivated);
}

void URotatingPlatformComponent::OnPlatformTriggerActivated(bool bIsActivated)
{
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

void URotatingPlatformComponent::TickPlatformTimeline(float Value)
{
	float NewValue = FMath::Lerp(First, Second, Value);
	FRotator NewRotation = CachedPlatformBase->GetActorRotation();
	NewRotation.Pitch = NewValue;
	CachedPlatformBase->SetActorRotation(NewRotation);
}


void URotatingPlatformComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	PlatformTimeline.TickTimeline(DeltaTime);
}

