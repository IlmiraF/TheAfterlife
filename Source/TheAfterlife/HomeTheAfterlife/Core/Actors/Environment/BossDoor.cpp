
#include "BossDoor.h"


ABossDoor::ABossDoor()
{
	PrimaryActorTick.bCanEverTick = true;

	TriggerComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerComponent"));
	RootComponent = TriggerComponent;
	TriggerComponent->SetCollisionProfileName(CollisionProfilePawnInteractionVolume);

	LeftDoor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftDoor"));
	LeftDoor->SetupAttachment(GetRootComponent());

	RightDoor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightDoor"));
	RightDoor->SetupAttachment(GetRootComponent());
}

void ABossDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	OpenDoors(DeltaTime);
}

void ABossDoor::OpenDoors(float DeltaTime)
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

void ABossDoor::BeginPlay()
{	
	Super::BeginPlay();

	Boss->OnBossIsDead.AddUObject(this, &ABossDoor::Open);
}

void ABossDoor::Open()
{	
	//GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Red, FString::Printf(TEXT("Boss Dead")));
	bStartOpen = true;

	//Destroy();
}
