// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SplineComponent.h"
#include "../../Actors/Environment/MeshActor.h"
#include "TramLine.generated.h"

UENUM(BlueprintType)
enum class ETramLineDirection : uint8
{
	Clockwise,
	Counterclockwise
};


UCLASS()
class THEAFTERLIFE_API ATramLine : public AActor
{
	GENERATED_BODY()
	
public:

    ATramLine();

    virtual void Tick(float DeltaTime) override;

protected:

    virtual void BeginPlay() override;

    UPROPERTY(EditAnywhere, Category = "Spline")
    USplineComponent* Spline;

    UPROPERTY(EditAnywhere, Category = "Trams")
    int AmountTram;

    UPROPERTY(EditAnywhere, Category = "Mesh")
    UStaticMesh* Mesh;

    UPROPERTY(EditAnywhere, Category = "Movement")
    ETramLineDirection MovementDirection = ETramLineDirection::Clockwise;

    UPROPERTY(EditAnywhere, Category = "Movement")
    float Speed = 300.f;

private:

    UPROPERTY(VisibleAnywhere, Category = "Movement")
    TArray<AMeshActor*> MovingTrams;

    void SpawnMeshes();

    void MoveMeshesAlongSpline(float DeltaTime);

    float GetSplineLength();
};
