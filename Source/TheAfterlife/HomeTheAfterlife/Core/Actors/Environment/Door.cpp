#include "../../../../TheAfterlifeTypes.h"
#include "Door.h"

ADoor::ADoor()
{
	PrimaryActorTick.bCanEverTick = true;

	TriggerComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerComponent"));
	TriggerComponent->SetupAttachment(GetRootComponent());

	TriggerComponent->SetCollisionProfileName(CollisionProfilePawnInteractionVolume);
	TriggerComponent->SetGenerateOverlapEvents(true);

	TriggerComponent->OnComponentBeginOverlap.AddDynamic(this, &ADoor::OnOverlapBegin);

	LeftDoor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftDoor"));
	LeftDoor->SetupAttachment(GetRootComponent());

	RightDoor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightDoor"));
	RightDoor->SetupAttachment(GetRootComponent());
}

void ADoor::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	bStartOpen = true;
}

void ADoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	OpenDoors(DeltaTime);
}

void ADoor::OpenDoors(float DeltaTime)
{
	if (!bStartOpen)
	{
		return;
	}

	if (bIsDoorOpened)
	{
		return;
	}

	FRotator LeftDoorRotation = LeftDoor->GetRelativeRotation();
	LeftDoorRotation.Yaw = FMath::FInterpTo(LeftDoorRotation.Yaw, -OpenAngle, DeltaTime, DoorOpenSpeed);
	LeftDoor->SetRelativeRotation(LeftDoorRotation);

	FRotator RightDoorRotation = RightDoor->GetRelativeRotation();
	RightDoorRotation.Yaw = FMath::FInterpTo(RightDoorRotation.Yaw, OpenAngle, DeltaTime, DoorOpenSpeed);
	RightDoor->SetRelativeRotation(RightDoorRotation);

	if (LeftDoorRotation.Yaw <= -OpenAngle && RightDoorRotation.Yaw >= OpenAngle)
	{
		bIsDoorOpened = true;
		SetActorTickEnabled(false);
	}
}



