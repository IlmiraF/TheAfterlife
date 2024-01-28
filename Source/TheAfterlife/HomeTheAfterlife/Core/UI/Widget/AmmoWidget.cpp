
#include "AmmoWidget.h"

int32 UAmmoWidget::GetAmmoCount() const
{
	return Ammo;
}

void UAmmoWidget::UpdateAmmoCount(int32 NewAmmo)
{
	Ammo = NewAmmo;

	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("UI  UpdateAmmoCount: %d"), Ammo));
}
