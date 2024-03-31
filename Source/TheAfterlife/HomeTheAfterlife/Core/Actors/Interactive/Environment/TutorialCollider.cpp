#include "TutorialCollider.h"
#include "../../../Characters/Birds/Bird.h"
#include "../../../Characters/PlayerCharacter.h"
#include "Components/BoxComponent.h"
#include "EngineUtils.h"
#include "../../../UI/Widget/HintsWidget.h"

ATutorialCollider::ATutorialCollider()
{
	PrimaryActorTick.bCanEverTick = true;
	Collider = CreateDefaultSubobject<UBoxComponent>(TEXT("Collider"));
	RootComponent = Collider;

	//Collider->OnComponentBeginOverlap.AddDynamic(this, &ATutorialCollider::OnOverlapBegin);
	//Collider->OnComponentEndOverlap.AddDynamic(this, &ATutorialCollider::OnOverlapEnd);
}

void ATutorialCollider::Interact(ABaseCharacter* Character)
{
	if (!IsValid(Character))
	{
		return;
	}

	Character->WidgetUpdateEvent.Broadcast(WidgetName, TutorialText, true);
}

bool ATutorialCollider::IsForce()
{
	return true;
}

//void ATutorialCollider::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
//{
//	if (!IsValid(OtherActor))
//	{
//		return;
//	}
//
//	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(OtherActor);
//
//	if (!IsValid(PlayerCharacter))
//	{
//		return;
//	}
//
//	PlayerCharacter->WidgetUpdateEvent.Broadcast(WidgetName, TutorialText, true);
//}
//
//void ATutorialCollider::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
//{
//	if (!IsValid(OtherActor))
//	{
//		return;
//	}
//
//	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(OtherActor);
//
//	if (!IsValid(PlayerCharacter))
//	{
//		return;
//	}
//
//	PlayerCharacter->WidgetUpdateEvent.Broadcast(WidgetName, TutorialText, false);
//}