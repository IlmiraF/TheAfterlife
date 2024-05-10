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
	//������������� �� ������� �� ���� � �������� ������ �� ������ �� ������ ��������� � ����������� �� �������
}

void UStateResolver::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	//������������ �� ������� �� ����
}
