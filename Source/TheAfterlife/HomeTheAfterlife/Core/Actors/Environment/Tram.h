#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SplineComponent.h"
#include "Components/BoxComponent.h"
#include "Components/TimelineComponent.h"
#include "Tram.generated.h"


UCLASS()
class THEAFTERLIFE_API ATram : public AActor
{
	GENERATED_BODY()
	
public:	
	
	ATram();

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spline")
	USplineComponent* SplineComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spline")
	UBoxComponent* MovementToPlatrofmTriggerComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spline")
	UBoxComponent* MovementFromPlatfromTriggerComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spline")
	FVector StopWorldLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UStaticMeshComponent* TramMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	float Speed = 300.0f;

	UFUNCTION()
	void TriggerToPlatfromOnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void TriggerFromlatfromOnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:	

	bool bIsMoving = false;
	float DistanceAlongSpline;

	void Move(float DeltaTime);

	float StopDistance;
};
