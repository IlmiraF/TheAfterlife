#include "DialoguePoint.h"
#include "../../Actors/Interfaces/ISpeak.h"
#include "../../Actors/Interfaces/ActionDuringSpeech.h"
#include "../../Characters/BaseCharacter.h"


ADialoguePoint::ADialoguePoint()
{
	PrimaryActorTick.bCanEverTick = true;

	Collider = CreateDefaultSubobject<UBoxComponent>(TEXT("Collider"));
	RootComponent = Collider;

	Collider->OnComponentBeginOverlap.AddDynamic(this, &ADialoguePoint::OnOverlapBegin);

	CurrentSpeechIndex = 0;
}

void ADialoguePoint::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bItSounded)
	{
		return;
	}

	CachedPlayerCharacter = Cast<APlayerCharacter>(OtherActor);
	CachedPlayerController = Cast<ABasePlayerController>(CachedPlayerCharacter->GetController());

	if (IsValid(CachedPlayerCharacter) && SpeechArray.Num() > 0)
	{
		CachedPlayerCharacter->SetCanMove(bCanMovePlayer);
		StartSpeech(CurrentSpeechIndex);
		bItSounded = true;
	}
}

void ADialoguePoint::StartSpeech(int32 SpeechIndex)
{
	if (!SpeechArray.IsValidIndex(SpeechIndex))
	{
		return;
	}

	FSpeechSettings& Speech = SpeechArray[SpeechIndex];

	if (Speech.CurrentSpeakerNotPlayer == nullptr)
	{
		CurrentSpeaker = CachedPlayerCharacter;
	}
	else
	{
		CurrentSpeaker = Speech.CurrentSpeakerNotPlayer;
	}

	if (CurrentSpeaker->Implements<USpeak>())
	{
		ISpeak* Speak = Cast<ISpeak>(CurrentSpeaker);

		Speak->Speak(SpeechArray[SpeechIndex].SoundBase);

		if (IsValid(CachedPlayerController))
		{
			CachedPlayerController->UpdateDialogueWidget(SpeechArray[SpeechIndex].SubtitlesText, true);
		}

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
		if (IsValid(CachedPlayerCharacter))
		{
			CachedPlayerCharacter->SetCanMove(true);
		}

		if (IsValid(CachedPlayerController))
		{
			CachedPlayerController->UpdateDialogueWidget("", false);
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