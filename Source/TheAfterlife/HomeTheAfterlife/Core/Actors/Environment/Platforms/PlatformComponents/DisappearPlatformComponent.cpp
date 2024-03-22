// Fill out your copyright notice in the Description page of Project Settings.


#include "DisappearPlatformComponent.h"
#include "../PlatformBase.h"
#include "Components/BoxComponent.h"

UDisappearPlatformComponent::UDisappearPlatformComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UDisappearPlatformComponent::BeginPlay()
{
	Super::BeginPlay();

	CachedPlatformBase = StaticCast<APlatformBase*>(GetOwner());

	CachedPlatformBase->GetTriggerBox()->OnComponentBeginOverlap.AddDynamic(this, &UDisappearPlatformComponent::OnTriggerOverlapBegin);
	CachedPlatformBase->GetTriggerBox()->OnComponentEndOverlap.AddDynamic(this, &UDisappearPlatformComponent::OnTriggerOverlapEnd);
	
	//BaseMaterial = UMaterialInstanceDynamic::Create(PlatformMesh->GetMaterial(0), nullptr);
	//PlatformMesh->SetMaterial(0, BaseMaterial);

	if (IsValid(ShineCurve))
	{
		FOnTimelineFloatStatic OnTimelineUpdate;
		OnTimelineUpdate.BindUFunction(this, FName("TickPlatformTimeline"));
		PlatformTimeline.AddInterpFloat(ShineCurve, OnTimelineUpdate);
	}
}

void UDisappearPlatformComponent::OnTriggerOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UDisappearPlatformComponent::Disappear, ShineDelay, true);
	PlatformTimeline.Play();
}

void UDisappearPlatformComponent::OnTriggerOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	PlatformTimeline.Reverse();
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
}

void UDisappearPlatformComponent::TickPlatformTimeline(float Value)
{
	//BaseMaterial->SetScalarParameterValue(FName("Shine"), Value * ShineIntensity);
}

void UDisappearPlatformComponent::Disappear()
{
	CachedPlatformBase->Destroy();
}

void UDisappearPlatformComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	PlatformTimeline.TickTimeline(DeltaTime);
}

