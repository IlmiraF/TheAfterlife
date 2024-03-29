#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "../../Characters/PlayerCharacter.h"
#include "../../Characters/Controllers/BasePlayerController.h"
#include "DialoguePoint.generated.h"

USTRUCT(BlueprintType)
struct FSpeechSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class USoundBase* SoundBase;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	AActor* CurrentSpeakerNotPlayer;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString SubtitlesText;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool ActionAtBeginningPhrase;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool ActionAtEndPhrase;
};

UCLASS()
class THEAFTERLIFE_API ADialoguePoint : public AActor
{
	GENERATED_BODY()
	
public:	
	ADialoguePoint();

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
	FTimerHandle ActionTimerHandle;

	int32 CurrentSpeechIndex;

	void StartSpeech(int32 SpeechIndex);

	void NextSpeech();

	APlayerCharacter* CachedPlayerCharacter;

	ABasePlayerController* CachedPlayerController;

	bool bItSounded = false;

	void StartAction();

	AActor* CurrentSpeaker;

};