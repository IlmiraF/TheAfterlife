#include "BossDialogueStart.h"

ABossDialogueStart::ABossDialogueStart()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ABossDialogueStart::BeginPlay()
{
	Super::BeginPlay();

	Boss->OnFirstStageCompleted.AddUObject(this, &ABossDialogueStart::StartSecondStage);
	Boss->OnBossIsDead.AddUObject(this, &ABossDialogueStart::BossDeath);

}

void ABossDialogueStart::StartSecondStage()
{
	SecondStageDialogueStart->StartDialogue();
}

void ABossDialogueStart::BossDeath()
{
	DeathBossDialogueStart->StartDialogue();
}