// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuWidget.h"
#include "Kismet/GameplayStatics.h"

void UMainMenuWidget::NativeConstruct()
{
	PlayerController = GetWorld()->GetFirstPlayerController();

	FInputModeUIOnly InputModeData;
	InputModeData.SetWidgetToFocus(this->TakeWidget());
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

	PlayerController->SetInputMode(InputModeData);
	PlayerController->bShowMouseCursor = true;
}

void UMainMenuWidget::LoadMainMenu()
{
}

void UMainMenuWidget::StartGame()
{
	UGameplayStatics::OpenLevel(GetWorld(), LobbyMapName, true, "listen");
}

void UMainMenuWidget::ShowSettings()
{

}

void UMainMenuWidget::ExitGame()
{

}

