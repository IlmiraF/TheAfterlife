#include "TutorialCollider.h"
#include "../../../Characters/BaseCharacter.h"
#include "Components/BoxComponent.h"

ATutorialCollider::ATutorialCollider()
{
	PrimaryActorTick.bCanEverTick = true;
	Collider = CreateDefaultSubobject<UBoxComponent>(TEXT("Collider"));
	RootComponent = Collider;
}

void ATutorialCollider::Interact(ABaseCharacter* Character)
{
	if (!IsValid(Character))
	{
		return;
	}

	Character->WidgetUpdateEvent.Broadcast(WidgetName, TutorialText, true);

	Destroy();
}

bool ATutorialCollider::IsForce()
{
	return true;
}