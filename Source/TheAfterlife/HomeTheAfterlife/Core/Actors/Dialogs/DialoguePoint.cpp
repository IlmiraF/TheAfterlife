#include "DialoguePoint.h"
#include "../../Actors/Interfaces/ISpeak.h"
#include "../../Actors/Interfaces/ActionDuringSpeech.h"
#include "Components/BoxComponent.h"
#include "../../Characters/BaseCharacter.h"


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

	if (CachedPlayerCharacter.IsValid() && (SpeechArray.Num() > 0))
	{
		CachedPlayerCharacter->SetCanMove(bCanMovePlayer);
		StartSpeech(CurrentSpeechIndex);
		bItSounded = true;
	}
}

bool ADialoguePoint::IsForce()
{
	return bIsForce;
}

void ADialoguePoint::StartSpeech(int32 SpeechIndex)
{
	if (!SpeechArray.IsValidIndex(SpeechIndex))
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

	if (CurrentSpeaker->Implements<USpeak>())
	{
		ISpeak* Speak = Cast<ISpeak>(CurrentSpeaker);

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
	}
}

void ADialoguePoint::NextSpeech()
{
	CurrentSpeechIndex++;

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
	}
}

void ADialoguePoint::StartAction()
{
	if (CurrentSpeaker->Implements<UActionDuringSpeech>())
	{
		IActionDuringSpeech* Action = Cast<IActionDuringSpeech>(CurrentSpeaker);
		Action->ActionDuringSpeech();
	}
}