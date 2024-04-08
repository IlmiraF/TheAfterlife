#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "../Interactive/Interactive.h"
#include "../../Actors/Interfaces/ISpeak.h"
#include "DialoguePoint.generated.h"

UENUM(BlueprintType)
enum class ESpeakerType : uint8
{
	None,
	Player,
	NotPlayer,
	Max UMETA(Hidden)
};

USTRUCT(BlueprintType)
struct FDialogueLine : public FTableRowBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USoundBase* Sound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString SubtitleText;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	ESpeakerType SpeakerType = ESpeakerType::None;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool ActionAtBeginningPhrase;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool ActionAtEndPhrase;
};

class UBoxComponent;
class ABaseCharacter;
class UDialogueWidget;
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

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	TMap<ESpeakerType, TSoftObjectPtr<AActor>> SpeakingActors;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bCanMovePlayer;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsForce = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UDataTable* SelectedDialogTable = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UDialogueWidget> DialogWidgetClass;

private:

	void StartDialogue();
	void ShowNextDialogueLine();
	void FinishDialogue();

	UDialogueWidget* DialogBox = nullptr;

	FTimerHandle SpeechTimerHandle;
	FTimerHandle ActionTimerHandle;

	int32 CurrentSpeechIndex;

	TWeakObjectPtr<ABaseCharacter> CachedPlayerCharacter;
	AActor* CurrentSpeaker = nullptr;

	bool bItSounded = false;

	void StartAction();
};