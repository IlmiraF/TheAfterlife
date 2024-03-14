
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
	TriggerComponent->OnComponentEndOverlap.AddDynamic(this, &ATram::OnOverlapEnd);
}

void ATram::ProcessMovementTimeline(float Value)
{
	const float SplineLenght = SplineComponent->GetSplineLength();

	FVector CurrentSplineLocation = SplineComponent->GetLocationAtDistanceAlongSpline(Value * SplineLenght, ESplineCoordinateSpace::World);
	FRotator CurrentSplineRotation = SplineComponent->GetRotationAtDistanceAlongSpline(Value * SplineLenght, ESplineCoordinateSpace::World);

	//SetActorLocationAndRotation(CurrentSplineLocation, CurrentSplineRotation);
	TramMesh->SetWorldLocationAndRotation(CurrentSplineLocation, CurrentSplineRotation);
}

void ATram::OnEndMovementTimeline()
{
}

void ATram::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	FOnTimelineFloat ProgressFuction;
	ProgressFuction.BindUFunction(this, TEXT("ProcessMovementTimeline"));
	MovementTimeline.AddInterpFloat(MovementCurve, ProgressFuction);

	FOnTimelineEvent OnTimelineFinishedFuction;
	OnTimelineFinishedFuction.BindUFunction(this, TEXT("OnEndMovementTimeline"));

	MovementTimeline.SetTimelineFinishedFunc(OnTimelineFinishedFuction);
	MovementTimeline.SetTimelineLengthMode(TL_LastKeyFrame);
	MovementTimeline.Play();

	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("GOIDA"));
}

void ATram::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

void ATram::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATram::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (MovementTimeline.IsPlaying())
	{
		MovementTimeline.TickTimeline(DeltaTime);
	}
}