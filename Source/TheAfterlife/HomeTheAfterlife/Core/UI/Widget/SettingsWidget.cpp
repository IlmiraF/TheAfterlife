// Fill out your copyright notice in the Description page of Project Settings.


#include "SettingsWidget.h"
#include "MainMenuWidget.h"
#include "SavesWidget.h"

void USettingsWidget::BackToMenu()
{
	this->RemoveFromParent();
	UMainMenuWidget* MainMenuWidget = CreateWidget<UMainMenuWidget>(GetWorld(), MainMenuWidgetClass);
	MainMenuWidget->AddToViewport();
}

void USettingsWidget::SavesMenu()
{
	this->RemoveFromParent();
	USavesWidget* SavesWidget = CreateWidget<USavesWidget>(GetWorld(), SavesWidgetClass);
	SavesWidget->AddToViewport();
}
