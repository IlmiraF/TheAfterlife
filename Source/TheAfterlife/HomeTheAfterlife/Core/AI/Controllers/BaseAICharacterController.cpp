// Fill out your copyright notice in the Description page of Project Settings.

#include "Perception/AISense_Sight.h"
#include "../Characters\BaseAICharacter.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "TheAfterlife\TheAfterlifeTypes.h"
#include "BaseAICharacterController.h"

void ABaseAICharacterController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);

	if (IsValid(InPawn))
	{
		checkf(InPawn->IsA<ABaseAICharacter>(), TEXT("AAICharacterController::SetPawn GCAICharacterController can possess only GCAICharacter"));
		CachedAICharacter = StaticCast<ABaseAICharacter*>(InPawn);
		RunBehaviorTree(CachedAICharacter->GetBehaviorTree());
	}
	else
	{
		CachedAICharacter = nullptr;
	}
}

void ABaseAICharacterController::ActorsPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	Super::ActorsPerceptionUpdated(UpdatedActors);

	if (!CachedAICharacter.IsValid())
	{
		return;
	}

	TryMoveToNextTarget();
}

void ABaseAICharacterController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	Super::OnMoveCompleted(RequestID, Result);

	if (!Result.IsSuccess())
	{
		return;
	}

	TryMoveToNextTarget();
}

void ABaseAICharacterController::BeginPlay()
{
	Super::BeginPlay();

	UAIPatrollingComponent* PatrollingComponent = CachedAICharacter->GetPatrollingComponent();

	if (PatrollingComponent->CanPatrol())
	{
		FVector ClosestWayPoint = PatrollingComponent->SelectClosestWayPoint();

		if (IsValid(Blackboard))
		{
			Blackboard->SetValueAsVector(BB_NextLocation, ClosestWayPoint);
			Blackboard->SetValueAsObject(BB_CurrentTarget, nullptr);
		}

		bIsPatrolling = true;
	}
}

void ABaseAICharacterController::TryMoveToNextTarget()
{
	UAIPatrollingComponent* PatrollingComponent = CachedAICharacter->GetPatrollingComponent();
	AActor* ClosestActor = GetClosestSesnedActor(UAISense_Sight::StaticClass());

	if (IsValid(ClosestActor))
	{
		if (IsValid(Blackboard))
		{
			Blackboard->SetValueAsObject(BB_CurrentTarget, ClosestActor);
			SetFocus(ClosestActor, EAIFocusPriority::Gameplay);
		}

		bIsPatrolling = false;
	}
	else if (PatrollingComponent->CanPatrol())
	{
		FVector WayPoint = FVector::ZeroVector;

		if (bIsPatrolling)
		{
			WayPoint = PatrollingComponent->SelectNextWayPoint();
		}
		else
		{
			WayPoint = PatrollingComponent->SelectClosestWayPoint();
		}

		if (IsValid(Blackboard))
		{
			ClearFocus(EAIFocusPriority::Gameplay);
			Blackboard->SetValueAsVector(BB_NextLocation, WayPoint);
			Blackboard->SetValueAsObject(BB_CurrentTarget, nullptr);
		}

		bIsPatrolling = true;
	}

}

bool ABaseAICharacterController::IsTargetReached(FVector TargetLocation) const
{
	return (TargetLocation - CachedAICharacter->GetActorLocation()).SizeSquared() <= FMath::Square(TargetReachRadius);
}
