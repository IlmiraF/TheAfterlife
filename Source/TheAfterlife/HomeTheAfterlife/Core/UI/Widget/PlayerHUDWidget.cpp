// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHUDWidget.h"
#include "Blueprint/WidgetTree.h"
#include "HintsWidget.h"
#include "../../Characters/BaseCharacter.h"
#include "../../Components/CharacterComponents/CharacterAttributeComponent.h"

UHintsWidget* UPlayerHUDWidget::GetHintsWidget()
{
	return WidgetTree->FindWidget<UHintsWidget>(HintsWidgetName);
}

//float UPlayerHUDWidget::GetHealthPercent() const
//{
//    float Result = 1.0f;
//    APawn* Pawn = GetOwningPlayerPawn();
//    ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(Pawn);
//    if (IsValid(BaseCharacter))
//    {
//        const UCharacterAttributeComponent* CharacterAttribute = BaseCharacter->GetCharacterAttributeComponent();
//        Result = CharacterAttribute->GetHealthPercent();
//    }
//    return Result;
//}
//
//float UPlayerHUDWidget::GetLeftBalancePercent() const
//{
//    float Result = 1.0f;
//    APawn* Pawn = GetOwningPlayerPawn();
//    ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(Pawn);
//    if (IsValid(BaseCharacter))
//    {
//        const UCharacterAttributeComponent* CharacterAttribute = BaseCharacter->GetCharacterAttributeComponent();
//        Result = CharacterAttribute->GetBalancePercent();
//
//        if (Result > 0.0f)
//        {
//            Result = 0.0f;
//        }
//    }
//    return FMath::Abs(Result / 45.0f);
//}
//
//float UPlayerHUDWidget::GetRightBalancePercent() const
//{
//    float Result = 1.0f;
//    APawn* Pawn = GetOwningPlayerPawn();
//    ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(Pawn);
//    if (IsValid(BaseCharacter))
//    {
//        const UCharacterAttributeComponent* CharacterAttribute = BaseCharacter->GetCharacterAttributeComponent();
//        Result = CharacterAttribute->GetBalancePercent();
//
//        if (Result < 0.0f)
//        {
//            Result = 0.0f;
//        }
//    }
//    return (Result / 45.0f);
//}