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
	UBoxComponent* TriggerComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UStaticMeshComponent* TramMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spline")
	UCurveFloat* MovementCurve;

	UFUNCTION()
	void ProcessMovementTimeline(float Value);

	UFUNCTION()
	void OnEndMovementTimeline();

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:
	
	virtual void BeginPlay() override;

private:	

	FTimeline MovementTimeline;
};
