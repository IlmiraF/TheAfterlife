#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "DialoguePoint.generated.h"

class UBoxComponent;

DECLARE_MULTICAST_DELEGATE(FOnPlayerEnterCollider);

UCLASS()
class THEAFTERLIFE_API ADialoguePoint : public AActor
{
	GENERATED_BODY()

public:
	ADialoguePoint();

	virtual void BeginPlay() override;

	UFUNCTION()
	void OnPlayerEnter(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	FOnPlayerEnterCollider OnPlayerEnterCollider;

protected:

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* Collider;
};