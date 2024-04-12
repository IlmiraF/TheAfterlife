

#include "AAltar.h"

AAltar::AAltar()
{
	PrimaryActorTick.bCanEverTick = true;

	AltarMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AltarMesh"));
	AltarMesh->SetupAttachment(GetRootComponent());
}


void AAltar::BeginPlay()
{
	Super::BeginPlay();

	Health = MaxHealth;

	OnTakeAnyDamage.AddDynamic(this, &AAltar::TakeAnyDamage);
}

void AAltar::TakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	UE_LOG(LogDamage, Warning, TEXT("AAltar::OnTakeAnyDamag %s recevied %.2f amount of damage from %s"), *DamagedActor->GetName(), Damage, *DamageCauser->GetName());
	Health = FMath::Clamp(Health - Damage, 0.0f, MaxHealth);
	if (Health <= 0)
	{
		Destroy();
	}
}

void AAltar::Destroy()
{	

	SetActorHiddenInGame(true);
	AltarMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	if (EnemyArray.Num() > 0)
	{
		for (ABaseAICharacter* Enemy : EnemyArray)
		{
			Enemy->DisableCharacter();
		}
	}

	if (OnAltarDestroyed.IsBound())
	{
		OnAltarDestroyed.Broadcast();
	}
}