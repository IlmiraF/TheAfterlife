// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../../Characters/BaseCharacter.h"
#include "SpikeTrap.generated.h"

UCLASS()
class THEAFTERLIFE_API ASpikeTrap : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpikeTrap();

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	//class USkeletalMeshComponent* TrapMesh;

	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animations")
	//UAnimMontage* AnimMontage;

	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animations")
	//float AnimationInterval = 5.0f;

	void SetCanAttack(bool CanAttack);

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack")
	//float Damage = 5.0f;

	//bool bCanAttack;

protected:

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collider")
	//class UBoxComponent* BoxCollider;

private:

	//FTimerHandle ReloadTimerHandle;

	//UAnimInstance* AnimInstance;

	void StartAttackAnimation();

	ABaseCharacter* CachedCharacter;

	void TryAttack();
};
