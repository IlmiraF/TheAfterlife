#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnProjectileHit, const FHitResult&, Hit, const FVector&, Direction);

UCLASS()
class THEAFTERLIFE_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	

	AProjectile();

	UFUNCTION(BlueprintCallable)
	void LaunchProjectile(FVector Diretion);

	UPROPERTY(BlueprintAssignable)
	FOnProjectileHit OnProjectileHit;

protected:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USphereComponent* CollisionComponent;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UProjectileMovementComponent* ProjectileMovementComponent;

	virtual void OnProjectileLaunched();

	virtual void BeginPlay() override;

private:

	UFUNCTION()
	void OnCollisionHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	void EnableCollision();

	FTimerHandle CollisionTimerHandle;

};
