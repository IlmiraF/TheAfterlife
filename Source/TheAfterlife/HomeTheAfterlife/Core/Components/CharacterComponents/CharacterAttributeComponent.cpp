#include "CharacterAttributeComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/WeakObjectPtrTemplates.h"
#include "Components/CapsuleComponent.h"
#include "..\Source\TheAfterlife\HomeTheAfterlife\Core\Characters\PlayerCharacter.h"


UCharacterAttributeComponent::UCharacterAttributeComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UCharacterAttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	GEngine->AddOnScreenDebugMessage(0, 0.5f, FColor::Red, FString::Printf(TEXT("Current Health: %f"), Health));
#if UE_BUILD_DEBUG || UE_BUILD_DEVELOPMENT
	void DebugDrawAttributes();
#endif

}

void UCharacterAttributeComponent::BeginPlay()
{
	Super::BeginPlay();

	checkf(GetOwner()->IsA<APlayerCharacter>(), TEXT("UCharacterAttributeComponent:: BeginPlay UCharacterAttributeComponent can be used only with APlayerCharacter"));
	CachedBaseCharacterOwner = StaticCast<APlayerCharacter*>(GetOwner());
	
	Health = MaxHealth;
	
	CachedBaseCharacterOwner->OnTakeAnyDamage.AddDynamic(this, &UCharacterAttributeComponent::OnTakeAnyDamage);
}

#if UE_BUILD_DEBUG || UE_BUILD_DEVELOPMENT
void UCharacterAttributeComponent::DebugDrawAttributes()
{

	//UDebugSubsystem* DebugSubsystem = UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem <UDebugSubsystem>();
	//
	//if (DebugSubsystem->IsCategoryEnabled(DebugCategoryAttributes) == false)
	//{
	//	return;
	//}
	//
	//FVector TextLocation = CachedBaseCharacterOwner->GetActorLocation() + (CachedBaseCharacterOwner->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() + 5.0f) * FVector::UpVector;
	//DrawDebugString(GetWorld(), TextLocation, FString::Printf(TEXT("Health: %.2f"), Health), nullptr, FColor::Green, 0.0f, true);
}
#endif



void UCharacterAttributeComponent::OnTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{	
	if (IsAlive() == false)
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
