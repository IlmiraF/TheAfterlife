// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ParkourLedge.generated.h"

class UBoxComponent;
class UStaticMeshComponent;
UCLASS()
class THEAFTERLIFE_API AParkourLedge : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AParkourLedge();

	FVector GetUpPointOfLocation() const;
	FVector GetDownPointOfLocation() const;

protected:

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Ledge parameters", meta = (MakeEditWidget))
	FVector UpPointOfLocation = FVector::ZeroVector;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Ledge parameters", meta = (MakeEditWidget))
	FVector DownPointOfLocation = FVector::ZeroVector;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* LedgeMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UBoxComponent* LedgeInteractionVolume;

};
