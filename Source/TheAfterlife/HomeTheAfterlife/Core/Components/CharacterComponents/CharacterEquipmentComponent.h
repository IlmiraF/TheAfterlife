// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "../../../../TheAfterlifeTypes.h"
#include "Components/ActorComponent.h"
#include "CharacterEquipmentComponent.generated.h"




class ARangeWeaponItem;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class THEAFTERLIFE_API UCharacterEquipmentComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	EEquipableItemType GetCurrentEquippedItemType() const;

	void Fire();

protected:
	
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "Loadout")
	TSubclassOf<ARangeWeaponItem> SideArmClass;

private:

	void CreateLoadout();

	ARangeWeaponItem* CurrentEquippedWeapon;

	TWeakObjectPtr<class ABaseCharacter> CachedBaseCharacter;
};
