
#include "BossDoor.h"

void ABossDoor::BeginPlay()
{
	Boss->OnBossIsDead.AddUObject(this, &ABossDoor::Open);
}

void ABossDoor::Open()
{
	bStartOpen = true;
}
