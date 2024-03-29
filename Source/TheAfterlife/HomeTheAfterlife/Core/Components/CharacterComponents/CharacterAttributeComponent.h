#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"
#include "../../Subsystems/SaveSubsystem/SaveSubsystemInterface.h"
#include "CharacterAttributeComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnDeathEventSignature)
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHealthChanged, float);
DECLARE_MULTICAST_DELEGATE(FOnHealthStealing);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class THEAFTERLIFE_API UCharacterAttributeComponent : public UActorComponent, public ISaveSubsystemInterface
{
	GENERATED_BODY()

public:
	UCharacterAttributeComponent();

	FOnDeathEventSignature OnDeathEvent;
	FOnHealthChanged OnHealthChangedEvent;

	FOnHealthStealing OnHealthStealingEvent;

	bool IsAlive() { return Health > 0.0f; }

	float GetHealthPercent() const;

	virtual void OnLevelDeserialized_Implementation() override;

	void AddHealth(float HealthToAdd);

	float GetBalancePercent() const;

	void StealHealth(float HealthStealingRatio);

	void RestoreHealth();

protected:

	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health", meta = (UIMin = 0.0f), SaveGame)
	float MaxHealth = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Health")
	USoundBase* HealthStealingAudio;

private:

	UPROPERTY(SaveGame)
	float Health = 0.0f;

	void OnHealthChanged();

	void OnHealthStealing();

	UFUNCTION()
	void OnTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	TWeakObjectPtr<class ABaseCharacter> CachedBaseCharacterOwner;
};
