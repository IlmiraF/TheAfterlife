#include "CharacterAttributeComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/WeakObjectPtrTemplates.h"
#include "Components/CapsuleComponent.h"
#include "../../Characters/BaseCharacter.h"
#include "Components/AudioComponent.h"
#include "../../Components/MovementComponents/BaseCharacterMovementComponent.h"


UCharacterAttributeComponent::UCharacterAttributeComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

float UCharacterAttributeComponent::GetHealthPercent() const
{
	return (Health / 100.0f);
}

float UCharacterAttributeComponent::GetBalancePercent() const
{
	return CachedBaseCharacterOwner->GetBaseCharacterMovementComponent()->GetOnBeamDirection();
}

void UCharacterAttributeComponent::StealHealth(float HealthStealingRatio)
{
	MaxHealth = MaxHealth * HealthStealingRatio;
	Health = FMath::Clamp(Health * HealthStealingRatio, 0.0f, MaxHealth);
	OnHealthStealing();
}

void UCharacterAttributeComponent::RestoreHealth()
{
	Health = MaxHealth;
}

void UCharacterAttributeComponent::SetInvulnerable(bool Value)
{
	Invulnerable = Value;
}

void UCharacterAttributeComponent::OnLevelDeserialized_Implementation()
{
	OnHealthChanged();
}

void UCharacterAttributeComponent::AddHealth(float HealthToAdd)
{
	Health = FMath::Clamp(Health + HealthToAdd, 0.0f, MaxHealth);
	OnHealthChanged();
}

void UCharacterAttributeComponent::ReduceHealth(float Damage)
{
	Health = FMath::Clamp(Health - Damage, 0.0f, MaxHealth);
	OnHealthChanged();
}

void UCharacterAttributeComponent::BeginPlay()
{
	Super::BeginPlay();

	checkf(MaxHealth > 0.0f,TEXT("UCharacterAttributeComponent:: BeginPlay max health cannot be equal to 0"));
	checkf(GetOwner()->IsA<ABaseCharacter>(), TEXT("UCharacterAttributeComponent:: BeginPlay UCharacterAttributeComponent can be used only with ABaseCharacter"));
	CachedBaseCharacterOwner = StaticCast<ABaseCharacter*>(GetOwner());

	Health = MaxHealth;

	CachedBaseCharacterOwner->OnTakeAnyDamage.AddDynamic(this, &UCharacterAttributeComponent::OnTakeAnyDamage);
}

void UCharacterAttributeComponent::OnHealthChanged()
{
	if (OnHealthChangedEvent.IsBound())
	{
		OnHealthChangedEvent.Broadcast(GetHealthPercent());
	}

	if (Health <= 0.0f)
	{
		//UE_LOG(LogDamage, Warning, TEXT("UCharacterAttributeComponent::OnTakeAnyDamage character %s is killed by an actor %s"), *CachedBaseCharacterOwner->GetName(), *DamageCauser->GetName());
		if (OnDeathEvent.IsBound())
		{
			OnDeathEvent.Broadcast();
		}
	}
}

void UCharacterAttributeComponent::OnHealthStealing()
{
	if (OnHealthStealingEvent.IsBound())
	{
		OnHealthStealingEvent.Broadcast();
	}

	if (HealthStealingAudio)
	{
		UGameplayStatics::PlaySoundAtLocation(this, HealthStealingAudio, GetOwner()->GetActorLocation());
	}
}

void UCharacterAttributeComponent::OnTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{	
	if (!IsAlive())
	{
		return;
	}

	if (Invulnerable)
	{
		return;
	}

	UE_LOG(LogDamage, Warning, TEXT("UCharacterAttributesComponent::OnTakeAnyDamage %s recevied %.2f amount of damage from %s"), *CachedBaseCharacterOwner->GetName(), Damage, *DamageCauser->GetName());
	Health = FMath::Clamp(Health - Damage, 0.0f, MaxHealth);

	OnHealthChanged();
}
