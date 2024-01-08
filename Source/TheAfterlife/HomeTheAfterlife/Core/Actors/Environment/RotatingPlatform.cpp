// Fill out your copyright notice in the Description page of Project Settings.


#include "RotatingPlatform.h"
#include "../../Characters/PlayerCharacter.h"

void ARotatingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FRotator NewRotation = GetActorRotation();

	NewRotation.Yaw += YawValue * DeltaTime;

	SetActorRotation(NewRotation);
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
	}
}

void ARotatingPlatform::OnTriggerOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnTriggerOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	SetActorTickEnabled(true);
	BaseCharacter = Cast<APlayerCharacter>(OtherActor);
	if (!BaseCharacter.IsValid())
	{
		return;
	}
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ARotatingPlatform::BlowWind, WindDelay, true);
}

void ARotatingPlatform::OnTriggerOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	GetWorldTimerManager().ClearTimer(TimerHandle);
	Super::OnTriggerOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}

void ARotatingPlatform::TickPlatformTimeline(float Value)
{
	FVector NewLocation = FMath::Lerp(StartLocation, EndLocation, Value);
	if (BaseCharacter.IsValid())
	{
		BaseCharacter->SetActorLocation(NewLocation);
	}
}

void ARotatingPlatform::BlowWind()
{
	StartLocation = BaseCharacter->GetActorLocation();
	FVector WindDirection = FVector::ForwardVector * FMath::RandRange(-1, 1) + FVector::RightVector * FMath::RandRange(-1, 1);
	EndLocation = StartLocation + WindDirection * WindForce;
	PlatformTimeline.PlayFromStart();
}
