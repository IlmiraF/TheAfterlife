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

void ABoss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SplineMovement(DeltaTime);
}

void ABoss::SetSplineMovement(bool Value)
{
	bIsMovingToSpline = Value;
}

void ABoss::SwitchSplines(EBirdFlinghtTypes FlyType)
{
	if (FlyType == EBirdFlinghtTypes::FlyingInCircle)
	{
		CachedSplineComponent = CircleSplineActor->GetSplineComponent();
		CurrentFlyType = EBirdFlinghtTypes::FlyingInCircle;
	}
	else
	{
		CachedSplineComponent = RiseSplineActor->GetSplineComponent();
		CurrentFlyType = EBirdFlinghtTypes::Rise;
	}
}

void ABoss::SetInvulnerable(bool Value)
{
	GetCharacterAttributeComponent() ->SetInvulnerable(Value);
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

	if (AmountAltars<=0)
	{
		FirstStageCompleted();
	}
}

void ABoss::FirstStageCompleted()
{
	if (OnFirstStageCompleted.IsBound())
	{
		OnFirstStageCompleted.Broadcast();
	}
}

void ABoss::SplineMovement(float DeltaTime)
{	
	if (!bIsMovingToSpline)
	{
		return;
	}

	if (!IsValid(CachedSkeletalMesh))
	{
		return;
	}

	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
	
	float NewDistance = DistanceAlongSpline + FlySpeed * DeltaTime;

	FVector NewLocation = CachedSplineComponent->GetLocationAtDistanceAlongSpline(NewDistance, ESplineCoordinateSpace::World);
	FRotator NewRotation = CachedSplineComponent->GetRotationAtDistanceAlongSpline(NewDistance, ESplineCoordinateSpace::World);

	SetActorLocationAndRotation(NewLocation, NewRotation);

	DistanceAlongSpline = NewDistance;
	float SplineLength = GetSplineLength();

	if (DistanceAlongSpline >= SplineLength)
	{
		MovementAlongCompleted();
	}
}

void ABoss::MovementAlongCompleted()
{
	if (CurrentFlyType == EBirdFlinghtTypes::FlyingInCircle)
	{
		DistanceAlongSpline = 0;
	}
	else if (CurrentFlyType == EBirdFlinghtTypes::Rise)
	{
		if (OnMovedToCircleSpline.IsBound())
		{
			OnMovedToCircleSpline.Broadcast();
		}
	}
}

float ABoss::GetSplineLength()
{
	float Length = 0.0f;
	int32 NumPoints = CachedSplineComponent->GetNumberOfSplinePoints();

	for (int32 i = 1; i < NumPoints; ++i)
	{
		FVector PrevPoint = CachedSplineComponent->GetLocationAtSplinePoint(i - 1, ESplineCoordinateSpace::World);
		FVector CurPoint = CachedSplineComponent->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::World);
		Length += FVector::Distance(PrevPoint, CurPoint);
	}

	return Length;
}