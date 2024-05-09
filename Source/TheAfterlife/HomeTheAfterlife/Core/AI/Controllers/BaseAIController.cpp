// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseAIController.h"
#include "Perception/AISense.h"
#include "Perception/AIPerceptionComponent.h"
#include "../../Characters/BaseCharacter.h"
#include "TheAfterlife/TheAfterlifeTypes.h"

ABaseAIController::ABaseAIController()
{
	PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));
}

AActor* ABaseAIController::GetClosestSesnedActor(TSubclassOf<UAISense> SenseClass) const
{

	if (!IsValid(GetPawn()))
	{
		return nullptr;
	}

	TArray<AActor*> SensedActors;
	PerceptionComponent->GetCurrentlyPerceivedActors(SenseClass, SensedActors);

	AActor* ClosestActor = nullptr;
	float MinSquaredDistance = FLT_MAX;
	FVector PawnLocation = GetPawn()->GetActorLocation();

	for (AActor* SensedActor : SensedActors)
	{	
		ABaseCharacter* SensedCharacter = Cast<ABaseCharacter>(SensedActor);

		if (!IsValid(SensedCharacter))
		{
			continue;
		}

		if (SensedCharacter->GetGenericTeamId() != FGenericTeamId((uint8)ETeams::PLAYER))
		{
			continue;
		}

		float CurrentSquaredDistance = (PawnLocation - SensedActor->GetActorLocation()).SizeSquared();
		if (CurrentSquaredDistance < MinSquaredDistance)
		{
			MinSquaredDistance = CurrentSquaredDistance;
			ClosestActor = SensedActor;
		}
	}
	return ClosestActor;
}
