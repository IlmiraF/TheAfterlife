// Fill out your copyright notice in the Description page of Project Settings.


#include "HintsWidget.h"

void UHintsWidget::UpdateHint(UTexture2D* NewHintlImage)
{
	HintImage = NewHintlImage;
}

void UHintsWidget::UpdateVisible(bool Visible)
{
	if (OnVisible.IsBound())
	{
		OnVisible.Broadcast(Visible);
	}
}