// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../../AI/Characters/BaseAICharacter.h"
#include "Components/BoxComponent.h"
#include "TheAfterlife/TheAfterlifeTypes.h"
#include "AAltar.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnAltarDestroyed);

UCLASS()
class THEAFTERLIFE_API AAltar : public AActor
{
	GENERATED_BODY()
	
public:	

	AAltar();

	FOnAltarDestroyed OnAltarDestroyed;

	void SetInvulnerable(bool Value);

protected:

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy")
	TArray<ABaseAICharacter*> EnemyArray;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UStaticMeshComponent* AltarMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Health", meta = (UIMin = 0.0f), SaveGame)
	float MaxHealth = 30.0f;

private:	

	float Health = 0.0f;

	UFUNCTION()
	void TakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	void Destroy();

	bool bInvulnerable = false;

};
