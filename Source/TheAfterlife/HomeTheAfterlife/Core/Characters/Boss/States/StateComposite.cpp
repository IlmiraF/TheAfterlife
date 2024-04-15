
#include "StateComposite.h"

void UStateComposite::Enter()
{
	for (UState* State : States)
	{
		State->Enter();
	}
}

void UStateComposite::Exit()
{
	for (UState* State : States)
	{
		State->Exit();
	}
}
