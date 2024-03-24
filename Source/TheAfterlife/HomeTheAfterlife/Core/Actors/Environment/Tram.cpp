
#include "../../../../TheAfterlifeTypes.h"
#include "Tram.h"
#include "../../Characters/BaseCharacter.h"
#include "../../Components/MovementComponents/BaseCharacterMovementComponent.h"
#include "../../Characters/PlayerCharacter.h"
#include "../../Characters/Controllers/BasePlayerController.h"

ATram::ATram()
{
	PrimaryActorTick.bCanEverTick = true;

	SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComponent"));
	SplineComponent->SetupAttachment(GetRootComponent());

	TramMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TramMesh"));
	TramMesh->SetupAttachment(SplineComponent);

	MovementToPlatrofmTriggerComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("MovementToPlatrofmTriggerComponent"));
	MovementToPlatrofmTriggerComponent->SetupAttachment(SplineComponent);

	MovementToPlatrofmTriggerComponent->SetCollisionProfileName(CollisionProfilePawnInteractionVolume);
	MovementToPlatrofmTriggerComponent->SetGenerateOverlapEvents(true);

	MovementFromPlatfromTriggerComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("MovementFromPlatfromTriggerComponent"));
	MovementFromPlatfromTriggerComponent->SetupAttachment(TramMesh);

	MovementFromPlatfromTriggerComponent->SetCollisionProfileName(CollisionProfilePawnInteractionVolume);
	MovementFromPlatfromTriggerComponent->SetGenerateOverlapEvents(true);

	MovementToPlatrofmTriggerComponent->OnComponentBeginOverlap.AddDynamic(this, &ATram::TriggerToPlatfromOnOverlapBegin);
	MovementFromPlatfromTriggerComponent->OnComponentBeginOverlap.AddDynamic(this, &ATram::TriggerFromlatfromOnOverlapBegin);
}

void ATram::TriggerToPlatfromOnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	bIsMoving = true;

	float InputKey = SplineComponent->FindInputKeyClosestToWorldLocation(StopWorldLocation);
	StopDistance = SplineComponent->GetDistanceAlongSplineAtSplineInputKey(InputKey);
}

void ATram::TriggerFromlatfromOnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	bIsMoving = true;

	StopDistance = INFINITY;

	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(OtherActor);
	if (IsValid(PlayerCharacter))
	{	
		PlayerCharacter->SetCanMove(false);
	}
}

void ATram::Move(float DeltaTime)
{
	if (!bIsMoving)
	{
		return;
	}

	if (DistanceAlongSpline >= StopDistance)
	{
		bIsMoving = false;
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