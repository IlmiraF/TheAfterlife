#include "PlayerHUDWidget.h"
#include "../../Characters\BaseCharacter.h"
#include "..\..\Components\CharacterComponents\CharacterAttributeComponent.h"

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
