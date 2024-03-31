// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Interactive.h"
#include "TutorialCollider.generated.h"

class UBoxComponent;
UCLASS()
class THEAFTERLIFE_API ATutorialCollider : public AActor, public IInteractable
{
	GENERATED_BODY()

public:

	ATutorialCollider();

	virtual void Interact(class ABaseCharacter* Character) override;
	virtual bool IsForce() override;

protected:

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* Collider;

	UPROPERTY(EditAnywhere)
	FString TutorialText;

	//UFUNCTION()
	//void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	//UFUNCTION()
	//void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName WidgetName = "WBP_Hints";
};