// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuWidget.h"
#include "Kismet/GameplayStatics.h"

void UMainMenuWidget::StartGame()
{
	UGameplayStatics::OpenLevel(GetWorld(), LevelName);
}

void UMainMenuWidget::GetSettings()
{}

void UMainMenuWidget::ExitGame()
{}