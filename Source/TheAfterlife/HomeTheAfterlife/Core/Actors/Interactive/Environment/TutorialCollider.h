// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Interactive.h"
#include "TutorialCollider.generated.h"

class UBoxComponent;
class ABaseCharacter;
UCLASS()
class THEAFTERLIFE_API ATutorialCollider : public AActor, public IInteractable
{
	GENERATED_BODY()

public:

	ATutorialCollider();

	virtual void Interact(ABaseCharacter* Character) override;
	virtual bool IsForce() override;

protected:

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* Collider;

	UPROPERTY(EditAnywhere)
	FString TutorialText;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName WidgetName = "WBP_Hints";

};