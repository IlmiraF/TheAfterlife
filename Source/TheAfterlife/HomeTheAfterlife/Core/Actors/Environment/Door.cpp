#include "Door.h"
#include "../../../../TheAfterlifeTypes.h"
#include "../../Characters/BaseCharacter.h"
#include "Components/BoxComponent.h"

ADoor::ADoor()
{
	PrimaryActorTick.bCanEverTick = true;

	SetActorTickEnabled(false);

	TriggerComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerComponent"));
	RootComponent = TriggerComponent;	
	TriggerComponent->SetCollisionProfileName(CollisionProfilePawnInteractionVolume);

	LeftDoor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftDoor"));
	LeftDoor->SetupAttachment(GetRootComponent());

	RightDoor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightDoor"));
	RightDoor->SetupAttachment(GetRootComponent());
}

void ADoor::Interact(ABaseCharacter* Character)
{
	SetActorTickEnabled(true);

	bStartOpen = true;

	if (OnDoorsOpening.IsBound())
	{
		OnDoorsOpening.Broadcast();
	}
}

bool ADoor::IsForce()
{
	return bIsForce;
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
