// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "../../Actors/Interfaces/ISpeak.h"
#include "Conductor.generated.h"


UCLASS()
class THEAFTERLIFE_API AConductor : public APawn, public IISpeakable
{
	GENERATED_BODY()

public:
	AConductor();

	//void StartSpeaking();

	//void StopSpeaking();

	virtual void Speak(USoundBase* SoundBase) override;

private:

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	class USkeletalMeshComponent* ConductorMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	UAudioComponent* ConductorAudioComponent;

	UPROPERTY(EditAnywhere, Category = "Speach")
	USoundBase* WealcomSpeech;

	UPROPERTY(EditAnywhere, Category = "Speach")
	TArray<USoundBase*> WaitingSpeeches;

	UPROPERTY(EditAnywhere, Category = "Speach")
	float WaitingTimeBetweenGreetingAndDiscontent = 120.0f;

	UPROPERTY(EditAnywhere, Category = "Speach")
	float WaitingTimeBetweenPhrasesDiscontent = 30.0f;

	FTimerHandle SpeechTimerHandle;
	int32 CurrentSpeechIndex;

	//void Speak();
};
