

#include "AAltar.h"

AAAltar::AAAltar()
{
	PrimaryActorTick.bCanEverTick = true;

	AltarMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AltarMesh"));
	AltarMesh->SetupAttachment(GetRootComponent());
}


void AAAltar::BeginPlay()
{
	Super::BeginPlay();

	Health = MaxHealth;

	OnTakeAnyDamage.AddDynamic(this, &AAAltar::TakeAnyDamage);
}

void AAAltar::TakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	UE_LOG(LogDamage, Warning, TEXT("AAltar::OnTakeAnyDamag %s recevied %.2f amount of damage from %s"), *DamagedActor->GetName(), Damage, *DamageCauser->GetName());
	Health = FMath::Clamp(Health - Damage, 0.0f, MaxHealth);
	if (Health <= 0)
	{
		Destroy();
	}
}

void AAAltar::Destroy()
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
}