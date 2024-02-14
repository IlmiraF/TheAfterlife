#include "AnimNotifyLaunchThrowable.h"
#include <TheAfterlife/HomeTheAfterlife/Core/Characters/BaseCharacter.h>
#include <TheAfterlife\HomeTheAfterlife\Core\Components\CharacterComponents\CharacterEquipmentComponent.h>

void UAnimNotifyLaunchThrowable::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	ABaseCharacter* CharacterOwner = Cast<ABaseCharacter>(MeshComp->GetOwner());

	if (!IsValid(CharacterOwner))
	{
		return;
	}

	CharacterOwner->GetCharacterEquipmentComponent_Mutable()->LaunchCurrentThrowableItem();
}
