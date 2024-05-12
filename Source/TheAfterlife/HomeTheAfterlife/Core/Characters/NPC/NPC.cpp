#include "NPC.h"
#include "Components/AudioComponent.h"

ANPC::ANPC()
{
	PrimaryActorTick.bCanEverTick = true;
	
	NPCMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	NPCMesh->SetupAttachment(GetRootComponent());
	
	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	AudioComponent->SetupAttachment(NPCMesh);
}

void ANPC::Speak(USoundBase* SoundBase)
{
	AudioComponent->SetSound(SoundBase);
	if (AudioComponent && !AudioComponent->IsPlaying())
	{
		AudioComponent->Play(0.f);
	}
}
