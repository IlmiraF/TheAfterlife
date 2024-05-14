#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../../Characters/Boss/Boss.h"
#include "DialogueStart.h"
#include "BossDialogueStart.generated.h"



UCLASS()
class THEAFTERLIFE_API ABossDialogueStart : public AActor
{
	GENERATED_BODY()
	
public:	
	ABossDialogueStart();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Boss")
	ABoss* Boss;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dialogs")
	ADialogueStart* SecondStageDialogueStart;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dialogs")
	ADialogueStart* DeathBossDialogueStart;

	virtual void BeginPlay() override;

private:

	void StartSecondStage();
	void BossDeath();
};
