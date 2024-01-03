#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"
#include "CharacterAttributeComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnDeathEventSignature)

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class THEAFTERLIFE_API UCharacterAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCharacterAttributeComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	FOnDeathEventSignature OnDeathEvent;

	bool IsAlive() { return Health > 0.0f; };

	float GetHealthPercnet() const;

protected:

	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health", meta = (UIMin = 0.0f))
	float MaxHealth = 100.0f;

private:

	float Health = 0.0f;

	UFUNCTION()
	void OnTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	TWeakObjectPtr<class ABaseCharacter> CachedBaseCharacterOwner;
};
