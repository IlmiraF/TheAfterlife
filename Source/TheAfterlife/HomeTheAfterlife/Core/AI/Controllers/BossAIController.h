// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseAIController.h"
#include "../../Characters/Boss/Boss.h"
#include "../../Characters/PlayerCharacter.h"
#include "BossAIController.generated.h"


UCLASS()
class THEAFTERLIFE_API ABossAIController : public ABaseAIController
{
	GENERATED_BODY()

public:

	virtual void SetPawn(APawn* InPawn) override;

	virtual void OnPossess(APawn* InPawn) override;

	virtual void ActorsPerceptionUpdated(const TArray<AActor*>& UpdatedActors) override;

protected:

	virtual void BeginPlay() override;

private:

	TWeakObjectPtr<ABoss> CachedAIBoss;

	APlayerCharacter* CachedPlayer;

	void SearchPlayer(const TArray<AActor*>& UpdatedActors);

	void FirstStageCompleted();

	void MovedToCircleSpline();

	void BossConcussed();

	void BossHasLanded();
};
