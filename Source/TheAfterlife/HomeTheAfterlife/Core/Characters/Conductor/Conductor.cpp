// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/AudioComponent.h"
#include "Conductor.h"

AConductor::AConductor()
{
	PrimaryActorTick.bCanEverTick = true;
	CurrentSpeechIndex = -1;

	ConductorMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	ConductorMesh->SetupAttachment(GetRootComponent());

	ConductorAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	ConductorAudioComponent->SetupAttachment(ConductorMesh);
}

void AConductor::StartSpeaking()
{
	ConductorAudioComponent->SetSound(WealcomSpeech);
	ConductorAudioComponent->Play();

	GetWorld()->GetTimerManager().SetTimer(SpeechTimerHandle, this, &AConductor::Speak, WaitingTimeBetweenGreetingAndDiscontent, false);
}

void AConductor::StopSpeaking()
{
	ConductorAudioComponent->Stop();
	CurrentSpeechIndex = 999;
}

void AConductor::Speak()
{	
	CurrentSpeechIndex++;
	if (CurrentSpeechIndex < WaitingSpeeches.Num())
	{	
		ConductorAudioComponent->SetSound(WaitingSpeeches[CurrentSpeechIndex]);
		ConductorAudioComponent->Play();
		GetWorld()->GetTimerManager().SetTimer(SpeechTimerHandle, this, &AConductor::Speak, WaitingTimeBetweenPhrasesDiscontent, false);
	}
}
