
#include "FlyingPath.h"

AFlyingPath::AFlyingPath()
{
	SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComponent"));
	SplineComponent->SetupAttachment(GetRootComponent());
}

USplineComponent* AFlyingPath::GetFlyingSpline()
{
	return SplineComponent;
}
