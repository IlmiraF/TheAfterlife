
#pragma once

#include "CoreMinimal.h"
#include "../EquipableItem.h"
#include "RangeWeaponItem.generated.h"


UCLASS(Blueprintable)
class THEAFTERLIFE_API ARangeWeaponItem : public AEquipableItem
{
	GENERATED_BODY()

public:

	ARangeWeaponItem();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USkeletalMeshComponent* WeaponMesh;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UWeaponBarellComponent* WeaponBarell;

};
