
#include "Bird.h"
#include "AIController.h"

ABird::ABird()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ABird::BeginPlay()
{
	Super::BeginPlay();
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

void ABird::SetNewPoint(int32 Index)
{
	if (Index > CurrnetIndex)
	{
		CurrnetIndex = Index;
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