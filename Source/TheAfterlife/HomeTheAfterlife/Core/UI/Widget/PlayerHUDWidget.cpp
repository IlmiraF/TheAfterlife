
#include "PlayerHUDWidget.h"
#include "../../Characters/BaseCharacter.h"
#include "../../Components/CharacterComponents/CharacterAttributeComponent.h"
#include "AmmoWidget.h"
#include "HintsWidget.h"
#include "Blueprint/WidgetTree.h"

UAmmoWidget* UPlayerHUDWidget::GetWidgetAmmo()
{	
	return WidgetTree->FindWidget<UAmmoWidget>(AmmoWidgetName);
}

void UPlayerHUDWidget::SetBombAmmo(int32 Ammo)
{	
	AmmoUI = Ammo;
}

UHintsWidget* UPlayerHUDWidget::GetHintsWidget()
{
	return WidgetTree->FindWidget<UHintsWidget>(HintsWidgetName);
}

UDialogueWidget* UPlayerHUDWidget::GetDialogueWidget()
{
    return WidgetTree->FindWidget<UDialogueWidget>(DialogueWidgetName);
}

int UPlayerHUDWidget::GetBombAmmo() const
{	
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
		Result = CharacterAttribute->GetHealthPercent();
	}

	return Result;
}

float UPlayerHUDWidget::GetLeftBalancePercent() const
{
    float Result = 1.0f;
    APawn* Pawn = GetOwningPlayerPawn();
    ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(Pawn);
    if (IsValid(BaseCharacter))
    {
        const UCharacterAttributeComponent* CharacterAttribute = BaseCharacter->GetCharacterAttributeComponent();
        Result = CharacterAttribute->GetBalancePercent();

        if (Result > 0.0f)
        {
            Result = 0.0f;
        }
    }
    return FMath::Abs(Result / 45.0f);
}

float UPlayerHUDWidget::GetRightBalancePercent() const
{
    float Result = 1.0f;
    APawn* Pawn = GetOwningPlayerPawn();
    ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(Pawn);
    if (IsValid(BaseCharacter))
    {
        const UCharacterAttributeComponent* CharacterAttribute = BaseCharacter->GetCharacterAttributeComponent();
        Result = CharacterAttribute->GetBalancePercent();

        if (Result < 0.0f)
        {
            Result = 0.0f;
        }
    }
    return (Result / 45.0f);
}