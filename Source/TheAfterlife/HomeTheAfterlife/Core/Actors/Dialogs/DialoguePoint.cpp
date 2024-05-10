#include "DialoguePoint.h"
#include "Components/BoxComponent.h"
#include "../../Characters/BaseCharacter.h"

ADialoguePoint::ADialoguePoint()
{
	PrimaryActorTick.bCanEverTick = true;

	Collider = CreateDefaultSubobject<UBoxComponent>(TEXT("Collider"));
	RootComponent = Collider;
}

void ADialoguePoint::Interact(ABaseCharacter* Character)
{
	if (bItSounded)
	{
		return;
	}

	if (OnInteractionEvent.IsBound())
	{
		OnInteractionEvent.Broadcast();
		Collider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

bool ADialoguePoint::IsForce()
{
	return bIsForce;
}

void ADialoguePoint::ShouldStopInteracting(bool bStop)
{
	bStopInteract = bStop;
}

FDelegateHandle ADialoguePoint::AddOnInteractionUFunction(UObject* Object, const FName& FunctionName)
{
	return OnInteractionEvent.AddUFunction(Object, FunctionName);
}

void ADialoguePoint::RemoveOnInteractionDelegate(FDelegateHandle DelegateHandle)
{
	OnInteractionEvent.Remove(DelegateHandle);
	Collider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void ADialoguePoint::StartAction()
{
	/*if (CurrentSpeaker->Implements<UActionDuringSpeech>())
	{
		IActionDuringSpeech* Action = Cast<IActionDuringSpeech>(CurrentSpeaker);
		Action->ActionDuringSpeech();
	}*/
}