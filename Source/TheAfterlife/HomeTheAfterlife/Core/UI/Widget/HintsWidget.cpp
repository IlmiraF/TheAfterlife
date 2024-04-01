// Fill out your copyright notice in the Description page of Project Settings.


#include "HintsWidget.h"

void UHintsWidget::UpdateWidget(FString Text, bool Visible)
{
	GetWorld()->GetTimerManager().ClearTimer(VisibilityTimer);

	Super::UpdateWidget(Text, Visible);

	GetWorld()->GetTimerManager().SetTimer(VisibilityTimer, this, &UHintsWidget::DisableHint, Timer);
}

void UHintsWidget::DisableHint()
{
	Hint = "";
	bIsVisible = false;
}
