#include "DialogPoint.h"
#include "../../Actors/Interfaces/ISpeak.h"
#include "../../Characters/BaseCharacter.h"



ADialogPoint::ADialogPoint()
{
	PrimaryActorTick.bCanEverTick = true;

	Collider = CreateDefaultSubobject<UBoxComponent>(TEXT("Collider"));
	RootComponent = Collider;

	Collider->OnComponentBeginOverlap.AddDynamic(this, &ADialogPoint::OnOverlapBegin);

	CurrentSpeechIndex = 0;
}

void ADialogPoint::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
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

void ADialogPoint::StartSpeech(int32 SpeechIndex)
{
	if (!SpeechArray.IsValidIndex(SpeechIndex))
	{
		return;
	}

	FSpeechSettings& Speech = SpeechArray[SpeechIndex];

	AActor* Speaker;
	if (Speech.CurrentSpeakerNotPlayer == nullptr)
	{
		Speaker = CachedPlayerCharacter;
	}
	else
	{
		Speaker = Speech.CurrentSpeakerNotPlayer;
	}

	if (Speaker->Implements<USpeak>())
	{
		ISpeak* Speak = Cast<ISpeak>(Speaker);

		if (Speak)
		{
			Speak->Speak(SpeechArray[SpeechIndex].SoundBase);

			if (IsValid(CachedPlayerController))
			{	
				CachedPlayerController->UpdateDialogueWidget(SpeechArray[SpeechIndex].SpeechText, true);
			}

			GetWorld()->GetTimerManager().SetTimer(SpeechTimerHandle, this, &ADialogPoint::NextSpeech, Speech.SoundBase->Duration, false);
		}
	}

}

void ADialogPoint::NextSpeech()
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
