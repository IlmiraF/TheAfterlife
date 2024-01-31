// Fill out your copyright notice in the Description page of Project Settings.

#include "../../../UI\Widget\PlayerHUDWidget.h"
#include "../../../UI\Widget\HintsWidget.h"
#include "PlayerUIComponent.h"


UPlayerUIComponent::UPlayerUIComponent()
{

	PrimaryComponentTick.bCanEverTick = true;
}

void UPlayerUIComponent::BeginPlay()
{
	Super::BeginPlay();
	CreateAndInitializeWidgets();
}


void UPlayerUIComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

UHintsWidget* UPlayerUIComponent::GetHintsWidget()
{
	return PlayerHUDWidget->GetHintsWidget();
}

void UPlayerUIComponent::CreateAndInitializeWidgets()
{
	if (!IsValid(PlayerHUDWidget))
	{
		PlayerHUDWidget = CreateWidget<UPlayerHUDWidget>(GetWorld(), PlayerHudWidgetClass);
		if (IsValid(PlayerHUDWidget))
		{
			PlayerHUDWidget->AddToViewport();
		}
	}

	UHintsWidget* HintsWidget = PlayerHUDWidget->GetHintsWidget();
	if (IsValid(HintsWidget))
	{
		HintsWidget->UpdateVisible(false);
	}
}

