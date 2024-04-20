#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SplineComponent.h"
#include "SplineActor.generated.h"


UCLASS()
class THEAFTERLIFE_API ASplineActor : public AActor
{
	GENERATED_BODY()
	
public:	

	ASplineActor();

	USplineComponent* GetSplineComponent();

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spline")
	USplineComponent* SplineComponent;
};
