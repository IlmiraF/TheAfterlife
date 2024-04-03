#include "MovingSplineActor.h"
#include "../../../../TheAfterlifeTypes.h"

AMovingSplineActor::AMovingSplineActor()
{
	PrimaryActorTick.bCanEverTick = true;

	SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComponent"));
	SplineComponent->SetupAttachment(GetRootComponent());

	TriggerComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerComponent"));
	TriggerComponent->SetupAttachment(GetRootComponent());

	TriggerComponent->SetCollisionProfileName(CollisionProfilePawnInteractionVolume);
	TriggerComponent->SetGenerateOverlapEvents(true);

	ActorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TramMesh"));
	ActorMesh->SetupAttachment(SplineComponent);

	TriggerComponent->OnComponentBeginOverlap.AddDynamic(this, &AMovingSplineActor::OnOverlapBegin);
}

void AMovingSplineActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	bIsMoving = true;
}


void AMovingSplineActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Move(DeltaTime);
}

void AMovingSplineActor::Move(float DeltaTime)
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

	ActorMesh->SetWorldLocationAndRotation(CurrentSplineLocation, CurrentSplineRotation);
}