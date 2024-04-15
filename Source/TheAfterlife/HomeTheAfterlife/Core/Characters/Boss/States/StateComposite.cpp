#include "StateComposite.h"

void UStateComposite::Enter()
{	
	Super::Enter();

	for (UState* State : States)
	{
		State->Enter();
	}
}

void UStateComposite::Exit()
{	
	Super::Exit();

	for (UState* State : States)
	{
		State->Exit();
	}
}