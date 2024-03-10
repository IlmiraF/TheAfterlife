// Fill out your copyright notice in the Description page of Project Settings.


#include "SavesWidget.h"
#include "MainMenuWidget.h"
#include "Kismet/GameplayStatics.h"
#include "../../Subsystems/SaveSubsystem/SaveSubsystem.h"

void USavesWidget::LoadSave(int32 NumOfSave)
{
	USaveSubsystem* SaveSubsystem = UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem<USaveSubsystem>();
	SaveSubsystem->LoadGameByIndex(NumOfSave);
}

FText USavesWidget::SetText(int32 NumOfSave)
{
	USaveSubsystem* SaveSubsystem = UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem<USaveSubsystem>();
	if (SaveSubsystem->GetNumOfSaves() < NumOfSave)
	{
		return FText::FromString(TEXT("Empty"));
	}
	return FText::FromString(TEXT("Save"));
}

void USavesWidget::BackToMenu()
{
	this->RemoveFromParent();
	UMainMenuWidget* MainMenuWidget = CreateWidget<UMainMenuWidget>(GetWorld(), MainMenuWidgetClass);
	MainMenuWidget->AddToViewport();
}