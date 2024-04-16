#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tram.generated.h"


class AConductor;
class UBoxComponent;
UCLASS()
class THEAFTERLIFE_API ATram : public AActor
{
	GENERATED_BODY()
	
public:	
	
	ATram();

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spline")
	class USplineComponent* SplineComponent;

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	bool StartAtBegin;

	UFUNCTION()
	void TriggerToPlatfromOnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void TriggerFromlatfromOnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Conductor")
	AConductor* Conductor;

protected:

	virtual void BeginPlay() override;

private:	

	bool bIsMoving = false;
	float DistanceAlongSpline;

	void Move(float DeltaTime);

	float StopDistance;

	bool bOnTramStarted = false;

	void StartMove();

};
