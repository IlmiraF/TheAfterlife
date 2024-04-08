
#include "Altar.h"

AAltar::AAltar()
{
	PrimaryActorTick.bCanEverTick = true;

	AltarMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AltarMesh"));
	AltarMesh->SetupAttachment(GetRootComponent());

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	CollisionBox->SetupAttachment(GetRootComponent());
}

void AAltar::BeginPlay()
{
	Super::BeginPlay();

	Health = MaxHealth;
}

void AAltar::OnTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
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
	for (ABaseAICharacter* Enemy : EnemyArray)
	{
		Enemy->DisableCharacter();
	}

	SetActorHiddenInGame(true);
	CollisionBox->SetCollisionProfileName(CollisionProfileNoCollision);
}