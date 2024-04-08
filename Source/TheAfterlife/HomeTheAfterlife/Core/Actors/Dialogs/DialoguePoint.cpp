#include "DialoguePoint.h"
#include "../../Actors/Interfaces/ISpeak.h"
#include "../../Actors/Interfaces/ActionDuringSpeech.h"
#include "Components/BoxComponent.h"
#include "../../Characters/BaseCharacter.h"
#include "../../UI/Widget/DialogueWidget.h"


ADialoguePoint::ADialoguePoint()
{
	PrimaryActorTick.bCanEverTick = true;

	Collider = CreateDefaultSubobject<UBoxComponent>(TEXT("Collider"));
	RootComponent = Collider;

	CurrentSpeechIndex = 0;
}

void ADialoguePoint::Interact(ABaseCharacter* Character)
{
	if (bItSounded)
	{
		return;
	}

	CachedPlayerCharacter = Character;

	if (!IsValid(SelectedDialogTable))
	{
		return;
	}

	Collider->ConditionalBeginDestroy();

	StartDialogue();

	/*if (CachedPlayerCharacter.IsValid() && (SpeechArray.Num() > 0))
	{
		CachedPlayerCharacter->SetCanMove(bCanMovePlayer);
		StartSpeech(CurrentSpeechIndex);
		bItSounded = true;
	}*/
}

bool ADialoguePoint::IsForce()
{
	return bIsForce;
}

void ADialoguePoint::StartDialogue()
{
	if (SelectedDialogTable)
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

void ADialoguePoint::ShowNextDialogueLine()
{
	GetWorld()->GetTimerManager().ClearTimer(SpeechTimerHandle);
	if (CurrentSpeechIndex < SelectedDialogTable->GetRowMap().Num())
	{
		const FName RowName = SelectedDialogTable->GetRowNames()[CurrentSpeechIndex];
		if (const FDialogueLine* Row = SelectedDialogTable->FindRow<FDialogueLine>(RowName, ""))
		{
			if (IsValid(Row->Sound))
			{
				CurrentSpeechIndex++;
				DialogBox->UpdateWidget(Row->SubtitleText, true);
				CurrentSpeaker = SpeakingActors.Find(Row->SpeakerType)->Get();
				if (IsValid(CurrentSpeaker) && CurrentSpeaker->Implements<UISpeakable>())
				{
					IISpeakable* SpeakableActor = Cast<IISpeakable>(CurrentSpeaker);
					SpeakableActor->Speak(Row->Sound);
				}
				else
				{
					CachedPlayerCharacter->Speak(Row->Sound);
				}
				GetWorld()->GetTimerManager().SetTimer(SpeechTimerHandle, this, &ADialoguePoint::ShowNextDialogueLine, Row->Sound->GetDuration());
			}
		}
	}
	else
	{
		FinishDialogue();
	}
}

void ADialoguePoint::FinishDialogue()
{
	DialogBox->RemoveFromParent();
	DialogBox->ConditionalBeginDestroy();
	DialogBox = nullptr;
	Destroy();
}

//void ADialoguePoint::StartSpeech(int32 SpeechIndex)
//{
	/*if (!SpeechArray.IsValidIndex(SpeechIndex))
	{
		return;
	}

	FSpeechSettings& Speech = SpeechArray[SpeechIndex];

	if (!IsValid(Speech.CurrentSpeakerNotPlayer))
	{
		CurrentSpeaker = CachedPlayerCharacter.Get();
	}
	else
	{
		CurrentSpeaker = Speech.CurrentSpeakerNotPlayer;
	}

	if (CurrentSpeaker->Implements<UISpeakable>())
	{
		IISpeakable* Speak = Cast<IISpeakable>(CurrentSpeaker);

		if (!SpeechArray[SpeechIndex].SoundBase)
		{
			return;
		}

		Speak->Speak(SpeechArray[SpeechIndex].SoundBase);


		CachedPlayerCharacter->WidgetUpdateEvent.Broadcast(WidgetName, SpeechArray[SpeechIndex].SubtitlesText, true);

		if (Speech.ActionAtBeginningPhrase)
		{
			StartAction();
		}
		else if (Speech.ActionAtEndPhrase)
		{
			GetWorld()->GetTimerManager().SetTimer(ActionTimerHandle, this, &ADialoguePoint::StartAction, Speech.SoundBase->Duration, false);
		}

		GetWorld()->GetTimerManager().SetTimer(SpeechTimerHandle, this, &ADialoguePoint::NextSpeech, Speech.SoundBase->Duration, false);
	}*/
//}

//void ADialoguePoint::NextSpeech()
//{
	/*CurrentSpeechIndex++;

	if (SpeechArray.IsValidIndex(CurrentSpeechIndex))
	{
		StartSpeech(CurrentSpeechIndex);
	}
	else
	{
		if (CachedPlayerCharacter.IsValid())
		{
			CachedPlayerCharacter->SetCanMove(true);
			CachedPlayerCharacter->WidgetUpdateEvent.Broadcast(WidgetName, "", false);
		}
	}*/
//}

void ADialoguePoint::StartAction()
{
	if (CurrentSpeaker->Implements<UActionDuringSpeech>())
	{
		IActionDuringSpeech* Action = Cast<IActionDuringSpeech>(CurrentSpeaker);
		Action->ActionDuringSpeech();
	}
}