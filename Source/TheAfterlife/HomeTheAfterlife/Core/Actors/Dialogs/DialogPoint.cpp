#include "DialogPoint.h"
#include "../../../../TheAfterlifeTypes.h"
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
	//AActor* CurrentSpeaker = SpeechArray[0].CurrentSpeaker;
	//
	//CachedBaseCharacter = Cast<ABaseCharacter>(OtherActor);
	//
	//if (IsValid(CachedBaseCharacter) && SpeechArray.Num() > 0)
	//{
	//	CachedBaseCharacter->SetCanMove(bCanMovePlayer);
	//	StartSpeech(CurrentSpeechIndex);
	//}

	if (OtherActor->Implements<USpeak>())
	{
		ISpeak* Speaker = Cast<ISpeak>(OtherActor);

		if (Speaker)
		{
			Speaker->Speak(SpeechArray[0].SoundBase);
		}
		
	}
}

void ADialogPoint::StartSpeech(int32 SpeechIndex)
{
	if (SpeechArray.IsValidIndex(SpeechIndex))
	{
		FSpeechSettings& Speech = SpeechArray[SpeechIndex];
		//AActor* Speaker = Speech.CurrentSpeaker ? Speech.CurrentSpeaker : Cast<AActor>(GetWorld()->GetFirstPlayerController()->GetPawn());

		//if (Speaker && Speech.SoundBase)
		//{
		//	Speaker->PlaySound(Speech.SoundBase);
		//	GetWorld()->GetTimerManager().SetTimer(SpeechTimerHandle, this, &ADialogPoint::NextSpeech, Speech.SoundBase->Duration, false);
		//}
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
		ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
		if (BaseCharacter && !bCanMovePlayer)
		{
			BaseCharacter->SetCanMove(true);
		}
	}
}
