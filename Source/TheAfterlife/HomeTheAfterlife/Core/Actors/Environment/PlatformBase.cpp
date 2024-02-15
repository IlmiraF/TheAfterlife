
#include "PlatformBase.h"
#include "Components/StaticMeshComponent.h"
#include "PlatformTrigger.h"
#include "Components/BoxComponent.h"

APlatformBase::APlatformBase()
{
	PrimaryActorTick.bCanEverTick = true;
	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	SetRootComponent(TriggerBox);
	PlatformMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlatformMesh"));
	PlatformMesh->SetupAttachment(TriggerBox);
}

void APlatformBase::BeginPlay()
{
	Super::BeginPlay();
	SetActorTickEnabled(false);
	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &APlatformBase::OnTriggerOverlapBegin);
	TriggerBox->OnComponentEndOverlap.AddDynamic(this, &APlatformBase::OnTriggerOverlapEnd);

	if (IsValid(PlatformTrigger))
	{
		PlatformTrigger->OnTriggerActivated.AddDynamic(this, &APlatformBase::OnPlatformTriggerActivated);
	}
}

void APlatformBase::OnTriggerOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

void APlatformBase::OnTriggerOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

void APlatformBase::OnPlatformTriggerActivated(bool bIsActivated)
{
	SetActorTickEnabled(true);
}
