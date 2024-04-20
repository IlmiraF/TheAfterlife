#include "SplineActor.h"

ASplineActor::ASplineActor()
{
	PrimaryActorTick.bCanEverTick = true;

	SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComponent"));
	SplineComponent->SetClosedLoop(true);
}

USplineComponent* ASplineActor::GetSplineComponent()
{
	return SplineComponent;
}