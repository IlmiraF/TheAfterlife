// Fill out your copyright notice in the Description page of Project Settings.

#include "Blueprint/WidgetTree.h"
#include "PlayerHUDWidget.h"
#include "../../UI\Widget\HintsWidget.h"

UHintsWidget* UPlayerHUDWidget::GetHintsWidget()
{
	return WidgetTree->FindWidget<UHintsWidget>(HintsWidgetName);
}
