#include "Bridge.h"
#include "../../../../TheAfterlifeTypes.h"
#include "Components/AudioComponent.h"

ABridge::ABridge()
{
	PrimaryActorTick.bCanEverTick = true;

	BridgeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftDoor"));
	BridgeMesh->SetupAttachment(GetRootComponent());

	BridgeAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	BridgeAudioComponent->SetupAttachment(BridgeMesh);
}

void ABridge::BeginPlay()
{
	Super::BeginPlay();
	
	Door->OnDoorsOpening.AddUObject(this, &ABridge::DisablingBridge);
}

void ABridge::DisablingBridge()
{
	BridgeMesh->SetVisibility(false);
	BridgeMesh->SetCollisionProfileName(CollisionProfileNoCollision);

	BridgeAudioComponent->SetSound(SoundDestruction);
	BridgeAudioComponent->Play();
}