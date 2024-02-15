#include "AIPatrollingComponent.h"


bool UAIPatrollingComponent::CanPatrol() const
{	
	return IsValid(PatrollingPath) && PatrollingPath->GetWayPoints().Num() > 0;
}

FVector UAIPatrollingComponent::SelectClosestWayPoint()
{
	FVector OwnerLocation = GetOwner()->GetActorLocation();
	const TArray<FVector> WayPoints = PatrollingPath->GetWayPoints();
	FTransform PathTransfrom = PatrollingPath->GetActorTransform();
	
	FVector ClosestWayPoint;
	float MinSqDistance = FLT_MAX;
	
	for (int32 i = 0; i < WayPoints.Num(); ++i)
	{
		FVector WayPointWorld = PathTransfrom.TransformPosition(WayPoints[i]);
		float CurrentSqDistance = (OwnerLocation - WayPointWorld).SizeSquared();
	
		if (CurrentSqDistance < MinSqDistance)
		{
			MinSqDistance = CurrentSqDistance;
			ClosestWayPoint = WayPointWorld;
			CurretWayPointIndex = i;
		}
	}
	
	return ClosestWayPoint;
}

FVector UAIPatrollingComponent::SelectNextWayPoint()
{
	++CurretWayPointIndex;
	
	const TArray<FVector> WayPoints = PatrollingPath->GetWayPoints();
	
	
	if (CurretWayPointIndex == PatrollingPath->GetWayPoints().Num())
	{
		CurretWayPointIndex = 0;
	}
	
	FTransform PathTransfrom = PatrollingPath->GetActorTransform();
	FVector WayPoint = PathTransfrom.TransformPosition(WayPoints[CurretWayPointIndex]);
	
	return WayPoint;
}

