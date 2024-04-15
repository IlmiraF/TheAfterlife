
#include "StateMachine.h"


void UStateMachine::BeginPlay()
{
	Super::BeginPlay();

	if (EnterOnEnable)
	{
		Enter();
	}
}

void UStateMachine::Enter()
{
	Super::Enter();

	if (IsValid(CurrentState))
	{
		CurrentState = FindState(CurrentStateType);
		CurrentState->Enter();
	}
}

void UStateMachine::Exit()
{
	Super::Exit();

	if (!IsValid(CurrentState))
	{
		CurrentState->Exit();
		CurrentState = nullptr;
	}
}

void UStateMachine::SwitchState(EBossStateType StateType)
{
	Exit();
	CurrentStateType = StateType;
	Enter();
}

UState* UStateMachine::FindState(EBossStateType StateType)
{
	for (FStateHolder Holder : States)
	{
		if (Holder.Type == StateType)
		{
			return Holder.State;
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("State with Type %d not found"), (int32)StateType);
	return nullptr;
}