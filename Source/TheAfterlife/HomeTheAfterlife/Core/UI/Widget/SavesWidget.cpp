// Fill out your copyright notice in the Description page of Project Settings.


#include "SavesWidget.h"
#include "MainMenuWidget.h"
#include "Kismet/GameplayStatics.h"
#include "../../Subsystems/SaveSubsystem/SaveSubsystem.h"

void USavesWidget::LoadSave(int32 NumOfSave)
{
	USaveSubsystem* SaveSubsystem = UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem<USaveSubsystem>();
	SaveSubsystem->LoadLastGame();
}

FText USavesWidget::SetText() const
{
	if (CheckExist())
	{
		return FText::FromString(TEXT("Save"));
	}
	return FText::FromString(TEXT("Empty"));
}

void USavesWidget::BackToMenu()
{
	this->RemoveFromParent();
	UMainMenuWidget* MainMenuWidget = CreateWidget<UMainMenuWidget>(GetWorld(), MainMenuWidgetClass);
	MainMenuWidget->AddToViewport();
}

bool USavesWidget::CheckExist() const
{
	USaveSubsystem* SaveSubsystem = UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem<USaveSubsystem>();
	if (SaveSubsystem->GetNumOfSaves() >= 1)
	{
		return true;
	}
	return false;
}
