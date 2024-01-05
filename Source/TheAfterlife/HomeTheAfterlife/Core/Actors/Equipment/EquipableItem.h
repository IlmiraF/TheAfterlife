#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../../../../TheAfterlifeTypes.h"
#include "EquipableItem.generated.h"

UCLASS(Abstract, NotBlueprintable)
class THEAFTERLIFE_API AEquipableItem : public AActor
{
	GENERATED_BODY()

public:

	EEquipableItemType GetItemType() const;

	FName GetUnEquippedSocketName() const;
	FName GetEquippedSocketName() const;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Equipable item")
	EEquipableItemType ItemType = EEquipableItemType::NONE;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Equipable item")
	FName UnEquippedSocketName = NAME_None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Equipable item")
	FName EquippedSocketName = NAME_None;
};
