#include "DialoguePoint.h"
#include "Components/BoxComponent.h"
#include "../../Characters/BaseCharacter.h"

ADialoguePoint::ADialoguePoint()
{
	PrimaryActorTick.bCanEverTick = true;

	Collider = CreateDefaultSubobject<UBoxComponent>(TEXT("Collider"));
	RootComponent = Collider;
}

void ADialoguePoint::BeginPlay()
{
	Super::BeginPlay();

	Collider->OnComponentBeginOverlap.AddDynamic(this, &ADialoguePoint::OnPlayerEnter);
}

void ADialoguePoint::OnPlayerEnter(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{	
	if (bVisited)
	{
		return;
	}

	if (OnPlayerEnterCollider.IsBound())
	{
		OnPlayerEnterCollider.Broadcast();
	}

	bVisited = true;
}