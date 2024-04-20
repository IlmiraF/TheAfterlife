#include "SplineActor.h"

ASplineActor::ASplineActor()
{
	PrimaryActorTick.bCanEverTick = true;

	SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComponent"));
}

USplineComponent* ASplineActor::GetSplineComponent()
{
	return SplineComponent;
}