
#include "AmmoWidget.h"

int32 UAmmoWidget::GetAmmoCount() const
{
	return Ammo;
}

void UAmmoWidget::UpdateAmmoCount(int32 NewAmmo)
{
	Ammo = NewAmmo;
}
