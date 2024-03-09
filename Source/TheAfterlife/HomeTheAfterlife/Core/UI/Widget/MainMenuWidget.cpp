// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Blueprint/WidgetTree.h"
#include "SettingsWidget.h"

void UMainMenuWidget::StartGame()
{
	UGameplayStatics::OpenLevel(GetWorld(), LevelName);
}

void UMainMenuWidget::GetSettings()
{
	this->RemoveFromParent();
	USettingsWidget* SettingsWidget = CreateWidget<USettingsWidget>(GetWorld(), SettingsWidgetClass);
	SettingsWidget->AddToViewport();
}

void UMainMenuWidget::ExitGame()
{
	TEnumAsByte<EQuitPreference::Type> QuitPreference = EQuitPreference::Quit; 
	UKismetSystemLibrary::QuitGame(GetWorld(), UGameplayStatics::GetPlayerController(GetWorld(), 0), QuitPreference, true);
}