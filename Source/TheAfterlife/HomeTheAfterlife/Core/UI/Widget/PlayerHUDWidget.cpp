#include "PlayerHUDWidget.h"
#include "../../Characters\BaseCharacter.h"
#include "..\..\Components\CharacterComponents\CharacterAttributeComponent.h"
#include "../../../Core\UI\Widget\AmmoWidget.h"
#include "Blueprint/WidgetTree.h"

UAmmoWidget* UPlayerHUDWidget::GetWidgetAmmo()
{	
	return WidgetTree->FindWidget<UAmmoWidget>(AmmoWidgetName);
}

void UPlayerHUDWidget::SetBombAmmo(int32 Ammo)
{	
	GEngine->AddOnScreenDebugMessage(0, 1.5f, FColor::Blue, FString::Printf(TEXT("PIPISI: %d"), Ammo));
	AmmoUI = Ammo;
}

int UPlayerHUDWidget::GetBombAmmo() const
{	
	GEngine->AddOnScreenDebugMessage(0, 1.5f, FColor::Red, FString::Printf(TEXT("PIPISI: %d"), AmmoUI));
	return AmmoUI;
}


float UPlayerHUDWidget::GetHealthPercent() const
{	
	float Result = 1.0f;
	APawn* Pawn = GetOwningPlayerPawn();
	ABaseCharacter* Character = Cast<ABaseCharacter>(Pawn);

	if (IsValid(Character))
	{
		const UCharacterAttributeComponent* CharacterAttribute = Character->GetCharacterAttributeComponent();
		Result = CharacterAttribute->GetHealthPercnet();
	}

	return Result;
}



