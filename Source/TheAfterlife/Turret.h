// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "HomeTheAfterlife/Core/Components/WeaponComponents/WeaponBarellComponent.h"
#include "Turret.generated.h"

UENUM(BlueprintType)
enum ETurretState: uint8
{
	SEARCHING,
	FIRING
};

UCLASS()
class THEAFTERLIFE_API ATurret : public APawn
{
	GENERATED_BODY()

public:
	ATurret();

	virtual void Tick(float DeltaTime) override;

	void SetCurrentTarget(AActor* NewTarget);

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* TurretBaseComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* TurretBarrelComponent;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UWeaponBarellComponent* WeaponBarell;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Turret parameters", meta = (ClampMin = 0.0f, UIMin = 0.0f))
	float BaseSearchingRotationRate = 60.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Turret parameters", meta = (ClampMin = 0.0f, UIMin = 0.0f))
	float BarellPitchRotationRate = 60.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Turret parameters", meta = (ClampMax = 90.0f, UIMax = 90.0f))
	float MaxBarellPitchBarell = 60.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Turret parameters", meta = (ClampMin = -30.0f, UIMin = -30.0f))
	float MinBarellPitchBarell = -30.0f;

private:

	void SearchingMovement(float DelataTime);
	void FiringMovement(float DealtaTime);

	void SetCurrentTurretState(ETurretState NewState);

	ETurretState CurrentTurretState = ETurretState::SEARCHING;

	AActor* CurrentTarget = nullptr;
};
