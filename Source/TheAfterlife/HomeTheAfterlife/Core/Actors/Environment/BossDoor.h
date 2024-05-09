#pragma once

#include "CoreMinimal.h"
#include "../../Characters/Boss/Boss.h"
#include "GameFramework/Actor.h"
#include "BossDoor.generated.h"


UCLASS()
class THEAFTERLIFE_API ABossDoor : public AActor
{
	GENERATED_BODY()

public:

	ABossDoor();

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Boss")
	ABoss* Boss;

protected:

	void Open();

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	UBoxComponent* TriggerComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UStaticMeshComponent* LeftDoor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UStaticMeshComponent* RightDoor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Doors")
	float DoorOpenSpeed = 20.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Doors")
	float OpenAngle = 120.0f;

	bool bStartOpen = false;

private:


	UPROPERTY(EditAnywhere)
	bool bIsForce = true;

	bool bIsDoorOpened = false;

	void OpenDoors(float DeltaTime);
};