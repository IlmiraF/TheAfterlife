// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "DialogueStart.generated.h"

UENUM(BlueprintType)
enum class ESpeakerType : uint8
{
	None,
	Player,
	Speaker1,
	Speaker2,
	Speaker3,
	Speaker4,
	Speaker5,
	Speaker6,
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

USTRUCT(BlueprintType)
struct FDialogueSettings
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<ESpeakerType, TSoftObjectPtr<AActor>> SpeakingActors;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UDataTable* SelectedDialogTable = nullptr;
};

class UDialogueWidget;
class IInteractable;
UCLASS()
class THEAFTERLIFE_API ADialogueStart : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADialogueStart();

	UFUNCTION()
	void StartDialogue();

protected:

	virtual void BeginPlay() override;

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bCanMovePlayer;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsForce = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<class UDialogueWidget> DialogWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FDialogueSettings DialogueSettings;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	AActor* TriggerActor;

private:	

	void ShowNextDialogueLine();
	void FinishDialogue();

	UDialogueWidget* DialogBox = nullptr;

	FTimerHandle SpeechTimerHandle;

	int32 CurrentSpeechIndex;

	AActor* CurrentSpeaker = nullptr;

	bool bItSounded = false;

	void UnSubscribeFromTrigger();

	UPROPERTY()
	TScriptInterface<IInteractable> Trigger;

	FDelegateHandle TriggerHandle;

};
