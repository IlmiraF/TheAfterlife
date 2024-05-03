// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogueStart.h"
#include "../../UI/Widget/DialogueWidget.h"
#include "../Interfaces/ISpeak.h"
#include "../Interactive/Interactive.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"

ADialogueStart::ADialogueStart()
{
	CurrentSpeechIndex = 0;
}

void ADialogueStart::BeginPlay()
{
	Super::BeginPlay();

	DialogueSettings.SpeakingActors.Add(ESpeakerType::Player, UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	if (Trigger.GetInterface())
	{
		TriggerHandle = Trigger->AddOnInteractionUFunction(this, FName("StartDialogue"));
	}
}

void ADialogueStart::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	if (PropertyChangedEvent.Property->GetName() == GET_MEMBER_NAME_STRING_CHECKED(ADialogueStart, TriggerActor))
	{
		Trigger = TriggerActor;
		if (Trigger.GetInterface())
		{
			if (!Trigger->HasOnInteractionCallback())
			{
				TriggerActor = nullptr;
				Trigger = nullptr;
			}
		}
		else
		{
			TriggerActor = nullptr;
			Trigger = nullptr;
		}
	}
}

void ADialogueStart::StartDialogue()
{
	if (DialogueSettings.SelectedDialogTable)
	{
		if (IsValid(DialogWidgetClass))
		{
			DialogBox = CreateWidget<UDialogueWidget>(GetWorld(), DialogWidgetClass);
			if (DialogBox)
			{
				DialogBox->AddToViewport();
				CurrentSpeechIndex = 0;

				ShowNextDialogueLine();
			}
		}
	}
}

void ADialogueStart::ShowNextDialogueLine()
{
	GetWorld()->GetTimerManager().ClearTimer(SpeechTimerHandle);
	if (CurrentSpeechIndex < DialogueSettings.SelectedDialogTable->GetRowMap().Num())
	{
		const FName RowName = DialogueSettings.SelectedDialogTable->GetRowNames()[CurrentSpeechIndex];
		if (const FDialogueLine* Row = DialogueSettings.SelectedDialogTable->FindRow<FDialogueLine>(RowName, ""))
		{
			if (IsValid(Row->Sound))
			{
				CurrentSpeechIndex++;
				DialogBox->UpdateWidget(Row->SubtitleText, true);
				CurrentSpeaker = DialogueSettings.SpeakingActors.Find(Row->SpeakerType)->Get();
				if (IsValid(CurrentSpeaker) && CurrentSpeaker->Implements<UISpeakable>())
				{
					IISpeakable* SpeakableActor = Cast<IISpeakable>(CurrentSpeaker);
					SpeakableActor->Speak(Row->Sound);
				}
				GetWorld()->GetTimerManager().SetTimer(SpeechTimerHandle, this, &ADialogueStart::ShowNextDialogueLine, Row->Sound->GetDuration());
			}
		}
	}
	else
	{
		FinishDialogue();
	}
}

void ADialogueStart::FinishDialogue()
{
	DialogBox->RemoveFromParent();
	DialogBox->ConditionalBeginDestroy();
	DialogBox = nullptr;
	UnSubscribeFromTrigger();
}

void ADialogueStart::UnSubscribeFromTrigger()
{
	if (TriggerHandle.IsValid() && Trigger.GetInterface())
	{
		Trigger->RemoveOnInteractionDelegate(TriggerHandle);
	}
}

