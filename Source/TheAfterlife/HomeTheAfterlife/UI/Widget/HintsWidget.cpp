// Fill out your copyright notice in the Description page of Project Settings.


#include "HintsWidget.h"

void UHintsWidget::UpdateHint(FString newHint)
{	
	Hint = newHint;
}

bool UHintsWidget::UpdateVisible(bool Visible)
{	
	bIsVisible = Visible;

	if (bIsVisible == true)
	{
		this->SetVisibility(ESlateVisibility::Visible);

		if (OnVisibleKurwa.IsBound())
		{
			OnVisibleKurwa.Broadcast();
		}
		
	}
	else
	{
		this->SetVisibility(ESlateVisibility::Hidden);
	}

	return bIsVisible;
}
