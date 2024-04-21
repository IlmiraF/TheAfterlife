#include "StateMachine.h"


void UStateMachine::BeginPlay()
{
	Super::BeginPlay();

	if (EnterOnEnable)
	{
		Enter();

		GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Black, FString::Printf(TEXT("EnterOnEnable")));
	}

	GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Red, FString::Printf(TEXT("BeginPlay")));
}

void UStateMachine::Enter()
{
	Super::Enter();

	if (IsValid(CurrentState))
	{
		//CurrentState = FindState(CurrentStateType);
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

//void UStateMachine::SwitchState(EBossStateType StateType)
//{
//	Exit();
//	CurrentStateType = StateType;
//	Enter();
//}

//void UStateMachine::AddState(EBossStateType Type, UState* State)
//{
//	FStateHolder StateHolder;
//
//	StateHolder.Type = Type;
//	StateHolder.State = State;
//
//	States.Add(StateHolder);
//	//GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Cyan, FString::Printf(TEXT("ADD NEW STATE")));
//	//GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Green, FString::Printf(TEXT("Num States: %d"), States.Num()));
//}

//UState* UStateMachine::FindState(EBossStateType StateType)
//{
//	for (FStateHolder Holder : States)
//	{
//		GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Green, FString::Printf(TEXT("FOR")));
//
//		if (Holder.Type == StateType)
//		{	
//			GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Yellow, FString::Printf(TEXT("Holder.Type == StateType")));
//			return Holder.State;
//		}
//	}
//
//	UE_LOG(LogTemp, Warning, TEXT("State with Type %d not found"), (int32)StateType);
//	return nullptr;
//}
//
