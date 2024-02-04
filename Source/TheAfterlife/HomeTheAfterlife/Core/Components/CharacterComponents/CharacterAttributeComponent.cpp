#include "CharacterAttributeComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/WeakObjectPtrTemplates.h"
#include "Components/CapsuleComponent.h"
#include "../../Characters/BaseCharacter.h"
#include "../../Components/MovementComponents/BaseCharacterMovementComponent.h"


UCharacterAttributeComponent::UCharacterAttributeComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

float UCharacterAttributeComponent::GetHealthPercent() const
{
	return Health / MaxHealth;
}

float UCharacterAttributeComponent::GetBalancePercent() const
{
	return CachedBaseCharacterOwner->GetBaseCharacterMovementComponent()->GetOnBeamDirection();
}

void UCharacterAttributeComponent::BeginPlay()
{
	Super::BeginPlay();

	checkf(GetOwner()->IsA<ABaseCharacter>(), TEXT("UCharacterAttributeComponent:: BeginPlay UCharacterAttributeComponent can be used only with ABaseCharacter"));
	CachedBaseCharacterOwner = StaticCast<ABaseCharacter*>(GetOwner());

	Health = MaxHealth;

	CachedBaseCharacterOwner->OnTakeAnyDamage.AddDynamic(this, &UCharacterAttributeComponent::OnTakeAnyDamage);
}

void UCharacterAttributeComponent::OnTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{	
	if (!IsAlive())
	{
		return;
	}

	UE_LOG(LogDamage, Warning, TEXT("UCharacterAttributesComponent::OnTakeAnyDamage %s recevied %.2f amount of damage from %s"), *CachedBaseCharacterOwner->GetName(), Damage, *DamageCauser->GetName());
	Health = FMath::Clamp(Health - Damage, 0.0f, MaxHealth);

	if (Health <= 0.0f)
	{	
		UE_LOG(LogDamage, Warning, TEXT("UCharacterAttributesComponent::OnTakeAnyDamage character %s is killed by an actor %s"), *CachedBaseCharacterOwner->GetName(), *DamageCauser->GetName());


		if (OnDeathEvent.IsBound())
		{
			OnDeathEvent.Broadcast();
		}
	}
}
