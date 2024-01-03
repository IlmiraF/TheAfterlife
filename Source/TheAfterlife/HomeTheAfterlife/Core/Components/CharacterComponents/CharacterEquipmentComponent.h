// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CharacterEquipmentComponent.generated.h"

class ARangeWeaponItem;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class THEAFTERLIFE_API UCharacterEquipmentComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "Loadout")
	TSubclassOf<ARangeWeaponItem> SideArmClass;

private:

	void CreateLoadout();

	ARangeWeaponItem* CurrentEquippedItem;

	TWeakObjectPtr<class ABaseCharacter> CachedBaseCharacter;
};
