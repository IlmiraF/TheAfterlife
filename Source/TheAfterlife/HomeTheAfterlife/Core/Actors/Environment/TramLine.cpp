#include "TramLine.h"


ATramLine::ATramLine()
{
	PrimaryActorTick.bCanEverTick = true;

    Spline = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));
}

void ATramLine::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//MoveMeshesAlongSpline(DeltaTime);
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

        //GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Cyan,FString::Printf( TEXT("StartDistance: %f"), StartDistance));

        FVector Location = Spline->GetLocationAtDistanceAlongSpline(StartDistance, ESplineCoordinateSpace::World);

        //GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Cyan, FString::Printf(TEXT("Location: %f; %f ; %f"), Location.X, Location.Y, Location.Z));

        FRotator Rotation = Spline->GetRotationAtDistanceAlongSpline(StartDistance, ESplineCoordinateSpace::World);

        AMeshActor* TramMesh = GetWorld()->SpawnActor<AMeshActor>();
        TramMesh->SetMesh(Mesh);
        TramMesh->SetActorLocationAndRotation(Location, Rotation);

        GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Cyan, FString::Printf(TEXT("Location: %f; %f ; %f"), TramMesh->GetActorLocation().X, TramMesh->GetActorLocation().Y, TramMesh->GetActorLocation().Z));

        TramMesh->AttachToComponent(Spline, FAttachmentTransformRules::KeepWorldTransform);

        MovingTrams.Add(TramMesh);
    }
}

void ATramLine::MoveMeshesAlongSpline(float DeltaTime)
{
    float Direction = (MovementDirection == ETramLineDirection::Clockwise) ? 1.0f : -1.0f;

    for (AMeshActor* MovingMesh : MovingTrams)
    {
        float DistanceToTravel = Speed * DeltaTime * Direction;

        if (DistanceToTravel >= GetSplineLength())
        {
            DistanceToTravel -= GetSplineLength();
        }
        else if (DistanceToTravel < 0.0f)
        {
            DistanceToTravel += GetSplineLength();
        }

        FVector Location = Spline->GetLocationAtDistanceAlongSpline(DistanceToTravel, ESplineCoordinateSpace::World);
        FRotator Rotation = Spline->GetRotationAtDistanceAlongSpline(DistanceToTravel, ESplineCoordinateSpace::World);

        MovingMesh->SetActorLocationAndRotation(Location, Rotation);
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