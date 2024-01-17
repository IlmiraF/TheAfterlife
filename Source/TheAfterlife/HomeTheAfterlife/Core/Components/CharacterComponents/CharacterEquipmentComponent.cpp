#include "CharacterEquipmentComponent.h"
#include "../../Actors\Equipment\Weapons\RangeWeaponItem.h"
#include "../../Characters\BaseCharacter.h"


ARangeWeaponItem* UCharacterEquipmentComponent::GetCurrentRangeWeapon() const
{
	return CurrentEquippedWeapon;
}

EEquipableItemType UCharacterEquipmentComponent::GetCurrentEquippedItemType() const
{
	EEquipableItemType Result = EEquipableItemType::NONE;

	if (IsValid(CurrentEquippedItem))
	{
		Result = CurrentEquippedItem->GetItemType();
	}

	return Result;
}

AMeleeWeaponItem* UCharacterEquipmentComponent::GetCurrentMeleeWeaponItem() const
{
	return CurrentMeleeWeaponItem;
}

bool UCharacterEquipmentComponent::IsEquipping() const
{
	return bIsEquipping;
}

void UCharacterEquipmentComponent::Fire()
{
	if (IsValid(CurrentEquippedWeapon))
	{
		CurrentEquippedWeapon->Fire();
	}
}

void UCharacterEquipmentComponent::EquipItemInSlot(EEquipmentSlots Slot)
{

	if (bIsEquipping)
	{
		return;
	}

	UnEquipCurrentItem();
	CurrentEquippedItem = ItemsArray[(uint32)Slot];
	CurrentEquippedWeapon = Cast<ARangeWeaponItem>(CurrentEquippedItem);
	CurrentThrowableItem = Cast<AThrowableItem>(CurrentEquippedItem);
	CurrentMeleeWeaponItem = Cast<AMeleeWeaponItem>(CurrentEquippedItem);

	if (IsValid(CurrentEquippedItem))
	{
		UAnimMontage* EquipMontage = CurrentEquippedItem->GetCharacterEquipAnimMontage();
		if (IsValid(EquipMontage))
		{
			bIsEquipping = true;
			UAnimInstance* CharacterAnimInstnce = CachedBaseCharacter->GetMesh()->GetAnimInstance();
			float EquipDuration = CharacterAnimInstnce->Montage_Play(EquipMontage, 1.0f, EMontagePlayReturnType::Duration);
			GetWorld()->GetTimerManager().SetTimer(EquipTimer, this, &UCharacterEquipmentComponent::EquipAnimationFinished, EquipDuration, false);
		}
		else
		{
			AttachCurrentItemToEquippedSocket();
		}
		CurrentEquippedSlot = Slot;
		CurrentEquippedItem->Equip();
	}

	if (IsValid(CurrentEquippedWeapon))
	{
		OnCurrentWeaponAmmoChangedHandle = CurrentEquippedWeapon->OnAmmoChanged.AddUFunction(this, FName("OnCurrentWeaponAmmoChanged"));
		//OnCurrentWeaponReloadedHandle = CurrentEquippedWeapon->OnReloadComplete.AddUFunction(this, FName("OnWeaponReloadComplete"));
		OnCurrentWeaponAmmoChanged(CurrentEquippedWeapon->GetAmmo());
	}
}

void UCharacterEquipmentComponent::AttachCurrentItemToEquippedSocket()
{	
	if (IsValid(CurrentEquippedItem))
	{
		CurrentEquippedItem->AttachToComponent(CachedBaseCharacter->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, CurrentEquippedItem->GetEquippedSocketName());
	}
}

void UCharacterEquipmentComponent::LaunchCurrentThrowableItem()
{
	if (IsValid(CurrentThrowableItem))
	{
		CurrentThrowableItem->Throw();
		bIsEquipping = false;
		EquipItemInSlot(PreviousEquippedSlot);
	}
}

void UCharacterEquipmentComponent::UnEquipCurrentItem()
{
	if (IsValid(CurrentEquippedItem))
	{
		CurrentEquippedItem->AttachToComponent(CachedBaseCharacter->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, CurrentEquippedItem->GetUnEquippedSocketName());
		CurrentEquippedItem->UnEquip();
	}
	if (IsValid(CurrentEquippedWeapon))
	{
		//CurrentEquippedWeapon->StopFire();
		//CurrentEquippedWeapon->EndReload(false);
		CurrentEquippedWeapon->OnAmmoChanged.Remove(OnCurrentWeaponAmmoChangedHandle);
		//CurrentEquippedWeapon->OnReloadComplete.Remove(OnCurrentWeaponReloadedHandle);
	}

	PreviousEquippedSlot = CurrentEquippedSlot;
	CurrentEquippedSlot = EEquipmentSlots::NONE;

}

void UCharacterEquipmentComponent::BeginPlay()
{
	Super::BeginPlay();

	checkf(GetOwner()->IsA<ABaseCharacter>(), TEXT("UCharacterEquipmentComponent::BeginPlay() CharacterEquipmentComponent can be used only with a BaseCharacter"));
	CachedBaseCharacter = StaticCast<ABaseCharacter*>(GetOwner());
	CreateLoadout();
}

void UCharacterEquipmentComponent::CreateLoadout()
{

	AmunitionArray.AddZeroed((uint32)EAmunitionType::MAX);
	for (const TPair<EAmunitionType, int32>& AmmoPair : MaxAmunitionAmount)
	{
		AmunitionArray[(uint32)AmmoPair.Key] = FMath::Max(AmmoPair.Value, 0);
	}

	ItemsArray.AddZeroed((uint32)EEquipmentSlots::MAX);
	for (const TPair<EEquipmentSlots, TSubclassOf<AEquipableItem>>& ItemPair : ItemsLoadout)
	{
		if (!IsValid(ItemPair.Value))
		{
			continue;
		}

		AEquipableItem* Item = GetWorld()->SpawnActor<AEquipableItem>(ItemPair.Value);
		Item->AttachToComponent(CachedBaseCharacter->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, Item->GetUnEquippedSocketName());
		Item->SetOwner(CachedBaseCharacter.Get());
		Item->UnEquip();
		ItemsArray[(uint32)ItemPair.Key] = Item;
	}

	//CurrentEquippedWeapon = GetWorld()->SpawnActor<ARangeWeaponItem>(SideArmClass);
	//CurrentEquippedWeapon->AttachToComponent(CachedBaseCharacter->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, "hand_right_collision");
}

void UCharacterEquipmentComponent::EquipAnimationFinished()
{
	bIsEquipping = false;
	AttachCurrentItemToEquippedSocket();
}


uint32 UCharacterEquipmentComponent::NextItemsArraySlotIndex(uint32 CurrentSlotIndex)
{
	if (CurrentSlotIndex == ItemsArray.Num() - 1)
	{
		return 0;
	}
	else
	{
		return CurrentSlotIndex + 1;
	}
}

uint32 UCharacterEquipmentComponent::PreviousItemsArraySlotIndex(uint32 CurrentSlotIndex)
{
	if (CurrentSlotIndex == 0)
	{
		return ItemsArray.Num() - 1;
	}
	else
	{
		return CurrentSlotIndex - 1;
	}
}

void UCharacterEquipmentComponent::OnCurrentWeaponAmmoChanged(int32 Ammo)
{
	if (OnCurrentWeaponAmmoChangedEvent.IsBound())
	{
		OnCurrentWeaponAmmoChangedEvent.Broadcast(Ammo, GetAvailableAmunitionForCurrentWeapon());
	}
}

int32 UCharacterEquipmentComponent::GetAvailableAmunitionForCurrentWeapon()
{
	//return AmunitionArray[(uint32)GetCurrentRangeWeapon()->GetAmmoType()];
	return 0; //TODO
}

void UCharacterEquipmentComponent::EquipNextItem()
{
	uint32 CurrentSlotIndex = (uint32)CurrentEquippedSlot;
	uint32 NextSlotIndex = NextItemsArraySlotIndex(CurrentSlotIndex);

	while (CurrentSlotIndex != NextSlotIndex
		&& IgnoreSlotsWhileSwitch.Contains((EEquipmentSlots)NextSlotIndex)
		&& !IsValid(ItemsArray[NextSlotIndex]))
	{
		NextSlotIndex = NextItemsArraySlotIndex(NextSlotIndex);
	}
	if (CurrentSlotIndex != NextSlotIndex)
	{
		EquipItemInSlot((EEquipmentSlots)NextSlotIndex);
	}
}

void UCharacterEquipmentComponent::EquipPreviousItem()
{
	uint32 CurrentSlotIndex = (uint32)CurrentEquippedSlot;
	uint32 PreviousSlotIndex = PreviousItemsArraySlotIndex(CurrentSlotIndex);

	while (CurrentSlotIndex != PreviousSlotIndex
		&& IgnoreSlotsWhileSwitch.Contains((EEquipmentSlots)PreviousSlotIndex)
		&& !IsValid(ItemsArray[PreviousSlotIndex]))
	{
		PreviousSlotIndex = PreviousItemsArraySlotIndex(PreviousSlotIndex);
	}
	if (CurrentSlotIndex != PreviousSlotIndex)
	{
		EquipItemInSlot((EEquipmentSlots)PreviousSlotIndex);
	}
}
