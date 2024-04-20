#include "Boss.h"
#include "GameFramework/CharacterMovementComponent.h"


ABoss::ABoss(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
}

UBehaviorTree* ABoss::GetBehaviorTree() const
{
	return BehaviorTree;
}

void ABoss::SpawnEnemy()
{
	EnemyPoolObject->MakeEnemisVisible();
}

bool ABoss::AltarsIntact()
{
	return AmountAltars > 0;
}


void ABoss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SplineMovement(DeltaTime);
}

void ABoss::BeginPlay()
{
	Super::BeginPlay();;

	GetMesh()->SetSkeletalMesh(FirstStageBossMesh);

	CachedSkeletalMesh = GetMesh();

	for (AAltar* Altar : Altars)
	{
		Altar->OnAltarDestroyed.AddUObject(this, &ABoss::DestructionAltars);
	}

	AmountAltars = Altars.Num();
}

void ABoss::DestructionAltars()
{
	AmountAltars--;
}

void ABoss::SplineMovement(float DeltaTime)
{	
	if (!CanFlyToSpline)
	{
		return;
	}

	if (!IsValid(CachedSkeletalMesh))
	{
		return;
	}

	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
	
	float NewDistance = DistanceAlongSpline + FlySpeed * DeltaTime;

	FVector NewLocation = SplineActor->GetSplineComponent()->GetLocationAtDistanceAlongSpline(NewDistance, ESplineCoordinateSpace::World);
	FRotator NewRotation = SplineActor->GetSplineComponent()->GetRotationAtDistanceAlongSpline(NewDistance, ESplineCoordinateSpace::World);

	SetActorLocationAndRotation(NewLocation, NewRotation);

	DistanceAlongSpline = NewDistance;
	float SplineLength = GetSplineLength();

	if (DistanceAlongSpline >= SplineLength)
	{
		DistanceAlongSpline = 0;
	}
}

float ABoss::GetSplineLength()
{
	float Length = 0.0f;
	int32 NumPoints = SplineActor->GetSplineComponent()->GetNumberOfSplinePoints();

	for (int32 i = 1; i < NumPoints; ++i)
	{
		FVector PrevPoint = SplineActor->GetSplineComponent()->GetLocationAtSplinePoint(i - 1, ESplineCoordinateSpace::World);
		FVector CurPoint = SplineActor->GetSplineComponent()->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::World);
		Length += FVector::Distance(PrevPoint, CurPoint);
	}

	return Length;
}