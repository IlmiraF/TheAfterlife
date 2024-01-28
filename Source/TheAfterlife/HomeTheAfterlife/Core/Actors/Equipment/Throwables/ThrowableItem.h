#pragma once

#include "CoreMinimal.h"
#include "../EquipableItem.h"
#include "ThrowableItem.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnAmmoChanged, int32);

UCLASS(Blueprintable)
class THEAFTERLIFE_API AThrowableItem : public AEquipableItem
{
	GENERATED_BODY()

public:

	void StartThrow();

	void Throw();

	int32 GetAmmo() const;

	void SetAmmo(int32 NewAmmo);

	bool CanThrow();

	FOnAmmoChanged OnAmmoChanged;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Throwables")
	TSubclassOf<class AProjectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animations | Weapon")
	UAnimMontage* ThrowItemMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Throwables", meta = (UIMin = -90.0f, UIMax = 90.0f, ClampMin = -90.0f, ClampMax = 90.0f));
	float ThrowAngle = 0.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Throwables | Ammo", meta = (UIMin = 0, ClampMin = 0));
	int32 MaxAmmo = 5;

private: 

	int32 CurrentAmmo = 1;
};
