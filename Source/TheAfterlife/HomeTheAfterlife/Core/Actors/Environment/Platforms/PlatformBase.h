// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlatformBase.generated.h"

class UBoxComponent;
class UStaticMeshComponent;
class APlatformTrigger;
UCLASS()
class THEAFTERLIFE_API APlatformBase : public AActor
{
	GENERATED_BODY()

public:
	APlatformBase();

	UBoxComponent* GetTriggerBox() const { return TriggerBox; }

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Platform movement")
	UStaticMeshComponent* PlatformMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UBoxComponent* TriggerBox;
};