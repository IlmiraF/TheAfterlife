#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
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

class APlayerCharacter;
class UBoxComponent;
UCLASS()
class THEAFTERLIFE_API ADialoguePoint : public AActor
{
	GENERATED_BODY()
	
public:	
	ADialoguePoint();

protected:

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* Collider;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FSpeechSettings> SpeechArray;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bCanMovePlayer;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName WidgetName = "WBP_DialogueWidget";

private:

	FTimerHandle SpeechTimerHandle;
	FTimerHandle ActionTimerHandle;

	int32 CurrentSpeechIndex;

	void StartSpeech(int32 SpeechIndex);

	void NextSpeech();

	TWeakObjectPtr<APlayerCharacter> CachedPlayerCharacter;
	TWeakObjectPtr<AActor> CurrentSpeaker;

	bool bItSounded = false;

	void StartAction();
};