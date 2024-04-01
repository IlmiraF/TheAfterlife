#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Interactive/Interactive.h"
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

class UBoxComponent;
class ABaseCharacter;
UCLASS()
class THEAFTERLIFE_API ADialoguePoint : public AActor, public IInteractable
{
	GENERATED_BODY()
	
public:	
	ADialoguePoint();

	virtual void Interact(ABaseCharacter* Character) override;
	virtual bool IsForce() override;

protected:

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* Collider;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FSpeechSettings> SpeechArray;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bCanMovePlayer;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName WidgetName = "WBP_DialogueWidget";

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsForce = true;

private:

	FTimerHandle SpeechTimerHandle;
	FTimerHandle ActionTimerHandle;

	int32 CurrentSpeechIndex;

	void StartSpeech(int32 SpeechIndex);

	void NextSpeech();

	TWeakObjectPtr<ABaseCharacter> CachedPlayerCharacter;
	AActor* CurrentSpeaker;

	bool bItSounded = false;

	void StartAction();
};