#include "DestroyingStone.h"

ADestroyingStone::ADestroyingStone()
{
	PrimaryActorTick.bCanEverTick = true;

	StoneMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StoneMesh"));
	StoneMesh->SetupAttachment(GetRootComponent());
}

void ADestroyingStone::BeginPlay()
{
	Super::BeginPlay();

	OnTakeAnyDamage.AddDynamic(this, &ADestroyingStone::TakeAnyDamage);
}

void ADestroyingStone::TakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{	
	GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Red, FString::Printf(TEXT("TakeAnyDamage")));

	if (DamageType->GetName() != FString("Default__DT_Explosion_C"))
	{
		return;
	}

	GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Red, FString::Printf(TEXT("Default__DT_Explosion_C")));

	Destroy();
}