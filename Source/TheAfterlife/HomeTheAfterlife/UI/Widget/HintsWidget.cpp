// Fill out your copyright notice in the Description page of Project Settings.


#include "HintsWidget.h"

void UHintsWidget::UpdateHint(FString newHint)
{	
	Hint = newHint;
}

void UHintsWidget::UpdateVisible(bool Visible)
{	
	if (OnVisible.IsBound())
	{
		OnVisible.Broadcast(Visible);
	}
}
