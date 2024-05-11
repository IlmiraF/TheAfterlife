// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogueStart.h"
#include "../../UI/Widget/DialogueWidget.h"
#include "../Interfaces/ISpeak.h"
#include "../Interactive/Interactive.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"

ADialogueStart::ADialogueStart()
{
	Collider = CreateDefaultSubobject<UBoxComponent>(TEXT("Collider"));
	RootComponent = Collider;
}

void ADialogueStart::BeginPlay()
{
	Super::BeginPlay();
	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	DialogueSettings.SpeakingActors.Add(ESpeakerType::Player, PlayerCharacter);
	CachedPlayerCharacter = Cast<APlayerCharacter>(PlayerCharacter);
}

void ADialogueStart::StartDialogue()
{	
	if (!bCanMovePlayer)
	{
		CachedPlayerCharacter->SetCanMove(false);
	}

	Collider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	bIsAlreadyFinished = false;
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
	if ((CurrentSpeechIndex < DialogueSettings.SelectedDialogTable->GetRowMap().Num()))
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
				if (Row->NeedPauseBeforePhrase)
				{
					GetWorld()->GetTimerManager().SetTimer(SpeechTimerHandle, this, &ADialogueStart::ShowNextDialogueLine, Row->PauseTime);
				}
				else
				{
					GetWorld()->GetTimerManager().SetTimer(SpeechTimerHandle, this, &ADialogueStart::ShowNextDialogueLine, Row->Sound->GetDuration());
				}
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
	bIsAlreadyFinished = true;
	GetWorld()->GetTimerManager().ClearTimer(SpeechTimerHandle);
	DialogBox->RemoveFromParent();
	DialogBox->ConditionalBeginDestroy();
	DialogBox = nullptr;
	Collider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CachedPlayerCharacter->SetCanMove(true);
}