#pragma once
#include "CoreMinimal.h"
#include "../../../../TheAfterlifeTypes.h"
#include "Components/ActorComponent.h"
#include "CharacterEquipmentComponent.generated.h"

typedef TArray<class AEquipableItem*, TInlineAllocator<(uint32)EEquipmentSlots::MAX>> TItemsArray;
typedef TArray<int32, TInlineAllocator<(uint32)EAmunitionType::MAX>> TAmunitonArray;

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnCurrentWeaponAmmoChanged, int32, int32)

class ARangeWeaponItem;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class THEAFTERLIFE_API UCharacterEquipmentComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	ARangeWeaponItem* GetCurrentRangeWeapon() const;

	EEquipableItemType GetCurrentEquippedItemType() const;

	bool IsEquipping() const;

	FOnCurrentWeaponAmmoChanged OnCurrentWeaponAmmoChangedEvent;

	void Fire();

	void EquipItemInSlot(EEquipmentSlots Slot);

	void AttachCurrentItemToEquippedSocket();

	void UnEquipCurrentItem();

	void EquipNextItem();
	void EquipPreviousItem();

protected:
	
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Loadout")
	TMap<EAmunitionType, int32> MaxAmunitionAmount;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Loadout")
	TMap<EEquipmentSlots, TSubclassOf<class AEquipableItem>> ItemsLoadout;

private:

	TAmunitonArray AmunitionArray;
	TItemsArray ItemsArray;

	void CreateLoadout();
	void EquipAnimationFinished();

	UFUNCTION()
	void OnCurrentWeaponAmmoChanged(int32 Ammo);

	int32 GetAvailableAmunitionForCurrentWeapon();

	ARangeWeaponItem* CurrentEquippedWeapon;

	TWeakObjectPtr<class ABaseCharacter> CachedBaseCharacter;

	FDelegateHandle OnCurrentWeaponAmmoChangedHandle;
	FDelegateHandle OnCurrentWeaponReloadedHandle;

	AEquipableItem* CurrentEquippedItem;
	EEquipmentSlots CurrentEquippedSlot;

	bool bIsEquipping = false;

	uint32 NextItemsArraySlotIndex(uint32 CurrentSlotIndex);
	uint32 PreviousItemsArraySlotIndex(uint32 CurrentSlotIndex);
};
