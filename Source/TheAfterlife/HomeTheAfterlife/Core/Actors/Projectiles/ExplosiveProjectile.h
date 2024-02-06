// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TheAfterlife/HomeTheAfterlife/Core/Actors/Projectiles/Projectile.h"
#include "ExplosiveProjectile.generated.h"


class UExplosiveComponent;

UCLASS()
class THEAFTERLIFE_API AExplosiveProjectile : public AProjectile
{
	GENERATED_BODY()

public:

	AExplosiveProjectile();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UExplosiveComponent* ExplosiveComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Explosion")
	float DetonationTime = 2.0f;

	virtual void OnProjectileLaunched() override;

private:

	FTimerHandle DetonationTimer;

	AController* GetContrller();

	void OnDetonationTimerElapsed();
};
