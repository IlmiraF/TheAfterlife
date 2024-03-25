#include "DialogueWidget.h"

void UDialogueWidget::UpdateDialogue(FString newSpeach)
{
	Speach = newSpeach;
}

void UDialogueWidget::UpdateVisible(bool Visible)
{
	if (OnVisible.IsBound())
	{
		OnVisible.Broadcast(Visible);
	}
}
