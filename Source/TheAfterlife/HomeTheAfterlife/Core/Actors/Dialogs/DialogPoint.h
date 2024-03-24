// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "../../../../TheAfterlifeTypes.h"
#include "DialogPoint.generated.h"


USTRUCT(BlueprintType)
struct FSpeechSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class USoundBase* SoundBase;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	ESpeakers CurrentSpeaker;
};

UCLASS()
class THEAFTERLIFE_API ADialogPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	ADialogPoint();

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* Collider;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FSpeechSettings> SpeechArray;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bCanMovePlayer;

private:

	FTimerHandle SpeechTimerHandle;

	int32 CurrentSpeechIndex;

	void StartSpeech(int32 SpeechIndex);

	void NextSpeech();

	//ABaseCharacter* CachedBaseCharacter;
};
