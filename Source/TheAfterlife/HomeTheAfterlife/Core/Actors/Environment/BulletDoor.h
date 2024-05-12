// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "BulletDoor.generated.h"

UCLASS()
class THEAFTERLIFE_API ABulletDoor : public AActor
{
	GENERATED_BODY()
	
public:	

	ABulletDoor();

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* Collider;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UStaticMeshComponent* StoneMesh;

protected:

	virtual void BeginPlay() override;

	UFUNCTION()
	void OnBulletEnter(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


private:

	UFUNCTION()
	void TakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);
};
