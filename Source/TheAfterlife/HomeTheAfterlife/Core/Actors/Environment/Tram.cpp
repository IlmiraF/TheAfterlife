
#include "../../../../TheAfterlifeTypes.h"
#include "Tram.h"

ATram::ATram()
{
	PrimaryActorTick.bCanEverTick = true;

	SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComponent"));
	SplineComponent->SetupAttachment(GetRootComponent());

	TriggerComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerComponent"));
	TriggerComponent->SetupAttachment(SplineComponent);

	TriggerComponent->SetCollisionProfileName(CollisionProfilePawnInteractionVolume);
	TriggerComponent->SetGenerateOverlapEvents(true);

	TramMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TramMesh"));
	TramMesh->SetupAttachment(SplineComponent);

	TriggerComponent->OnComponentBeginOverlap.AddDynamic(this, &ATram::OnOverlapBegin);
}

void ATram::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	bIsMoving = true;
}

void ATram::Move(float DeltaTime)
{
	if (!bIsMoving)
	{
		return;
	}

	DistanceAlongSpline += Speed * DeltaTime;
	const float SplineLength = SplineComponent->GetSplineLength();
	DistanceAlongSpline = FMath::Min(DistanceAlongSpline, SplineLength);

	FVector CurrentSplineLocation = SplineComponent->GetLocationAtDistanceAlongSpline(DistanceAlongSpline, ESplineCoordinateSpace::World);
	FRotator CurrentSplineRotation = SplineComponent->GetRotationAtDistanceAlongSpline(DistanceAlongSpline, ESplineCoordinateSpace::World);

	TramMesh->SetWorldLocationAndRotation(CurrentSplineLocation, CurrentSplineRotation);
}

void ATram::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Move(DeltaTime);
}