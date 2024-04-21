#include "BossAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "TheAfterlife/TheAfterlifeTypes.h"

void ABossAIController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);

	if (IsValid(InPawn))
	{
		checkf(InPawn->IsA<ABoss>(), TEXT("AAICharacterController::SetPawn GCAICharacterController can possess only GCAICharacter"));
		CachedAIBoss = StaticCast<ABoss*>(InPawn);
	}
	else
	{
		CachedAIBoss = nullptr;
	}
}

void ABossAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
}

void ABossAIController::ActorsPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	Super::ActorsPerceptionUpdated(UpdatedActors);

	SearchPlayer(UpdatedActors);
}

void ABossAIController::BeginPlay()
{
	Super::BeginPlay();

	CachedAIBoss->OnFirstStageCompleted.AddUObject(this, &ABossAIController::FirstStageCompleted);
	CachedAIBoss->OnMovedToCircleSpline.AddUObject(this, &ABossAIController::MovedToCircleSpline);
}

void ABossAIController::SearchPlayer(const TArray<AActor*>& UpdatedActors)
{
	if (IsValid(CachedPlayer))
	{
		return;
	}

	for (AActor* Actor : UpdatedActors)
	{
		APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(Actor);

		if (PlayerCharacter && PlayerCharacter->IsPlayerControlled())
		{
			CachedPlayer = PlayerCharacter;
			RunBehaviorTree(CachedAIBoss->GetBehaviorTree());
			CachedAIBoss->SpawnEnemy();
			break;
		}
	}

}

void ABossAIController::FirstStageCompleted()
{
	Blackboard->SetValueAsBool(BB_AltarsDestroyed, false);
}

void ABossAIController::MovedToCircleSpline()
{
	Blackboard->SetValueAsBool(BB_OnCircleSpline, true);
}

