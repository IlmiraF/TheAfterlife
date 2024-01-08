#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

UCLASS()
class THEAFTERLIFE_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	

	AProjectile();

	UFUNCTION(BlueprintCallable)
	void LaunchProjectile(FVector Diretion);

protected:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USphereComponent* CollisionComponent;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UProjectileMovementComponent* ProjectileMovementComponent;

	virtual void OnProjectileLaunched();

};
