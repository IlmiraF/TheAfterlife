
#pragma once

#include "CoreMinimal.h"
#include "../EquipableItem.h"
#include "RangeWeaponItem.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnAmmoChanged, int32);

UCLASS(Blueprintable)
class THEAFTERLIFE_API ARangeWeaponItem : public AEquipableItem
{
	GENERATED_BODY()

public:

	ARangeWeaponItem();

	void Fire();

	EAmunitionType GetAmmoType() const;

	int32 GetAmmo() const;
	int32 GetMaxAmmo() const;

	void SetAmmo(int32 NewAmmo);

	FOnAmmoChanged OnAmmoChanged;

	bool CanShoot() const;

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USkeletalMeshComponent* WeaponMesh;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UWeaponBarellComponent* WeaponBarell;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon | Parameters | Ammo", meta = (ClampMin = 1, UIMin = 1))
	int32 MaxAmmo = 30;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon | Parameters | Ammo")
	EAmunitionType AmmoType;

private: 

	int32 Ammo = 0;

};
