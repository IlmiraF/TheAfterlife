#include "TriggerButton.h"

ATriggerButton::ATriggerButton()
{
	PrimaryActorTick.bCanEverTick = true;

	TriggerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TriggerMesh"));
	TriggerMesh->SetupAttachment(GetRootComponent());
}


void ATriggerButton::BeginPlay()
{
	Super::BeginPlay();

	OnTakeAnyDamage.AddDynamic(this, &ATriggerButton::TakeAnyDamage);
}

void ATriggerButton::TakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (bButtonIsActive)
	{
		return;
	}

	if (OnTriggerActivated.IsBound())
	{
		OnTriggerActivated.Broadcast(true);
		GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Red, FString::Printf(TEXT("BLOP")));
	}

	bButtonIsActive = true;
}