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
	return (Health / MaxHealth);
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

void UCharacterAttributeComponent::ClearDamageCounters()
{
	for (uint8 i = 1; i < (uint8)EDamageType::MAX; ++i)
	{
		DamageCounters[i] = 0;
	}
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

	UE_LOG(LogDamage, Warning, TEXT("AAltar::OnTakeAnyDamag %s recevied %.2f amount of damage from %s  type %s"), *DamagedActor->GetName(), Damage, *DamageCauser->GetName(), *DamageType->GetName());
	Health = FMath::Clamp(Health - Damage, 0.0f, MaxHealth);

	UpdateDamageCounters(DamageType->GetName(), Damage);

	OnHealthChanged();
}

void UCharacterAttributeComponent::UpdateDamageCounters(FString DamageType, float Damage)
{
	if (DamageType == FString("DT_Bullet"))
	{
		DamageCounters[(uint8)EDamageType::Bullet] += Damage;
	}
	else if (DamageType == FString("DT_Explosive"))
	{
		DamageCounters[(uint8)EDamageType::Explosive] += Damage;
	}
	else if (DamageType == FString("DT_Melee"))
	{
		DamageCounters[(uint8)EDamageType::Melee] += Damage;
	}
}

EDamageType UCharacterAttributeComponent::GetMostDamagingType() const
{
	EDamageType MostDamagingType = EDamageType::Bullet;
	float MaxDamage = DamageCounters[(uint8)EDamageType::Bullet];

	for (uint8 i = 1; i < (uint8)EDamageType::MAX; ++i)
	{
		if (DamageCounters[i] > MaxDamage)
		{
			MaxDamage = DamageCounters[i];
			MostDamagingType = (EDamageType)i;
		}
	}

	return MostDamagingType;
}


