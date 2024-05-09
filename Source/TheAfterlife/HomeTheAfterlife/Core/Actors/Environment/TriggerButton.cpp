#include "TriggerButton.h"

ATriggerButton::ATriggerButton()
{
	PrimaryActorTick.bCanEverTick = true;
}


void ATriggerButton::TakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (bButtonIsActive)
	{
		return;
	}

	if (OnButtonActivated.IsBound())
	{
		OnButtonActivated.Broadcast();
	}

	bButtonIsActive = true;
}