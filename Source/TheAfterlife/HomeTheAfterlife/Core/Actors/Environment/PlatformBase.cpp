
#include "PlatformBase.h"
#include "Components/StaticMeshComponent.h"
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
}

void APlatformBase::OnTriggerOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

void APlatformBase::OnTriggerOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}
