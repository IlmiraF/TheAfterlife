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
	CachedAIBoss->OnBossConcussed.AddUObject(this, &ABossAIController::BossConcussed);
	CachedAIBoss->OnBossHasLanded.AddUObject(this, &ABossAIController::BossHasLanded);;
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
			Blackboard->SetValueAsObject(BB_CurrentTarget, CachedPlayer);
			CachedAIBoss->SpawnEnemy();
			break;
		}
	}

}

void ABossAIController::FirstStageCompleted()
{
	Blackboard->SetValueAsBool(BB_AltarsDestroyed, true);
	Blackboard->SetValueAsBool(BB_IsBoy, true);
	CachedPlayer->GetCharacterAttributeComponent()->RestoreHealth();
}

void ABossAIController::MovedToCircleSpline(bool MovedToSpline)
{	
	Blackboard->SetValueAsBool(BB_OnCircleSpline, MovedToSpline);
}

void ABossAIController::BossConcussed(bool Concussed)
{	
	Blackboard->SetValueAsBool(BB_BossConcussed, Concussed);
}

void ABossAIController::BossHasLanded(bool Landed)
{
	Blackboard->SetValueAsBool(BB_OnGround, Landed);
}

void ABossAIController::BecomeBoy(bool Value)
{
	Blackboard->SetValueAsBool(BB_IsBoy, Value);
}