// Fill out your copyright notice in the Description page of Project Settings.


#include "TextableWidget.h"

void UTextableWidget::UpdateWidget(FString Text, bool Visible)
{
	Hint = Text;
	bIsVisible = Visible;
}
