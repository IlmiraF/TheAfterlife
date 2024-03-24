#include "DialogPoint.h"
#include "../../Characters/BaseCharacter.h"


ADialogPoint::ADialogPoint()
{

	PrimaryActorTick.bCanEverTick = true;

	Collider = CreateDefaultSubobject<UBoxComponent>(TEXT("Collider"));
	RootComponent = Collider;

	Collider->OnComponentBeginOverlap.AddDynamic(this, &ADialogPoint::OnOverlapBegin);
}

void ADialogPoint::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AActor* CurrentSpeaker = SpeechArray[0].CurrentSpeaker;

	ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(OtherActor);

	if (IsValid(BaseCharacter))
	{	
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, TEXT("VOLODYA"));
		BaseCharacter->SetCanMove(bCanMovePlayer);
	}

	if (SpeechArray[0].CurrentSpeaker == nullptr)
	{
		if (IsValid(BaseCharacter))
		{
			BaseCharacter->SetAudio(SpeechArray[0].SoundBase);
		}
	}
}