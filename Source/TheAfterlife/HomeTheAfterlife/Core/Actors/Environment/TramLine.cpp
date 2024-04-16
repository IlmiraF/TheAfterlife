#include "TramLine.h"


ATramLine::ATramLine()
{
	PrimaryActorTick.bCanEverTick = true;

	Spline = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));
}

void ATramLine::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MoveMeshesAlongSpline(DeltaTime);
}

void ATramLine::BeginPlay()
{
	Super::BeginPlay();

	SpawnMeshes();
}

void ATramLine::SpawnMeshes()
{
	float Direction = (MovementDirection == ETramLineDirection::Clockwise) ? 1.0f : -1.0f;

	float SplineLength = GetSplineLength();
	float SegmentLength = SplineLength / AmountTram;

	for (int32 i = 0; i < AmountTram; ++i)
	{
		float StartDistance = i * SegmentLength * Direction;

		FVector Location = Spline->GetLocationAtDistanceAlongSpline(StartDistance, ESplineCoordinateSpace::World);

		FRotator Rotation = Spline->GetRotationAtDistanceAlongSpline(StartDistance, ESplineCoordinateSpace::World);

		AMeshActor* TramMesh = GetWorld()->SpawnActor<AMeshActor>();
		TramMesh->SetMesh(Mesh);
		TramMesh->SetActorLocationAndRotation(Location, Rotation);
		TramMesh->SetDistanceAlongSpline(StartDistance);

		TramMesh->AttachToComponent(Spline, FAttachmentTransformRules::KeepWorldTransform);

		MovingTrams.Add(TramMesh);
	}
}

void ATramLine::MoveMeshesAlongSpline(float DeltaTime)
{
	float Direction = (MovementDirection == ETramLineDirection::Clockwise) ? 1.0f : -1.0f;
	float SplineLength = GetSplineLength();

	for (AMeshActor* MovingMesh : MovingTrams)
	{
		float CurrentDistance = MovingMesh->GetDistanceAlongSpline();
		float NewDistance = CurrentDistance + Speed * DeltaTime * Direction;

		if (NewDistance >= SplineLength)
		{
			NewDistance -= SplineLength;
		}
		else if (NewDistance < 0.0f)
		{
			NewDistance += SplineLength;
		}
		FVector NewLocation = Spline->GetLocationAtDistanceAlongSpline(NewDistance, ESplineCoordinateSpace::World);
		FRotator NewRotation = Spline->GetRotationAtDistanceAlongSpline(NewDistance, ESplineCoordinateSpace::World);

		MovingMesh->SetActorLocationAndRotation(NewLocation, NewRotation);
		MovingMesh->SetDistanceAlongSpline(NewDistance);
	}
}

float ATramLine::GetSplineLength()
{
	float Length = 0.0f;
	int32 NumPoints = Spline->GetNumberOfSplinePoints();

	for (int32 i = 1; i < NumPoints; ++i)
	{
		FVector PrevPoint = Spline->GetLocationAtSplinePoint(i - 1, ESplineCoordinateSpace::World);
		FVector CurPoint = Spline->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::World);
		Length += FVector::Distance(PrevPoint, CurPoint);
	}

	return Length;
}