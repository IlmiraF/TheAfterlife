#include "CharacterEquipmentComponent.h"
#include "../../Actors\Equipment\Weapons\RangeWeaponItem.h"
#include "../../Characters\BaseCharacter.h"


void UCharacterEquipmentComponent::BeginPlay()
{
	Super::BeginPlay();

	checkf(GetOwner()->IsA<ABaseCharacter>(), TEXT("UCharacterEquipmentComponent::BeginPlay() CharacterEquipmentComponent can be used only with a BaseCharacter"));
	CachedBaseCharacter = StaticCast<ABaseCharacter*>(GetOwner());
	CreateLoadout();
}

void UCharacterEquipmentComponent::CreateLoadout()
{	
	if (!IsValid(SideArmClass))
	{
		return;
	}
	CurrentEquippedItem = GetWorld()->SpawnActor<ARangeWeaponItem>(SideArmClass);
	CurrentEquippedItem->AttachToComponent(CachedBaseCharacter->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, "hand_right_collision");
}



