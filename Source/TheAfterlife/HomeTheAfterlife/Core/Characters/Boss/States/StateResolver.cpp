#include "StateResolver.h"
#include "../Source/TheAfterlife/TheAfterlifeTypes.h"

UStateResolver::UStateResolver()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UStateResolver::BeginPlay()
{
	Super::BeginPlay();

	StateMachine = GetOwner()->GetComponentByClass<UStateMachine>();
	//Подписываемся на события из мира и вызываем методы со сменой на нужное состояние в зависимости от события
}

void UStateResolver::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	//Отписываемся от событий из мира
}
