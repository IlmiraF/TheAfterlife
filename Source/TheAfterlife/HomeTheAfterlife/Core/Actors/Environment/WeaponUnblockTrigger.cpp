#include "WeaponUnblockTrigger.h"
#include "../../Characters/BaseCharacter.h"
#include "../../Components/CharacterComponents/CharacterEquipmentComponent.h"
#include "../../Characters/PlayerCharacter.h"

AWeaponUnblockTrigger::AWeaponUnblockTrigger()
{
	PrimaryActorTick.bCanEverTick = true;

	Collider = CreateDefaultSubobject<UBoxComponent>(TEXT("Collider"));
	RootComponent = Collider;

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
}

void AWeaponUnblockTrigger::BeginPlay()
{
	Super::BeginPlay();

	Collider->OnComponentBeginOverlap.AddDynamic(this, &AWeaponUnblockTrigger::OnPlayerEnter);
}


void AWeaponUnblockTrigger::OnPlayerEnter(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ABaseCharacter* Player = Cast<ABaseCharacter>(OtherActor);
	if (!IsValid(Player))
	{
		return;
	}

	if (UnblockType == EUnblockType::Bomb)
	{	
		Player->UnblockBomb();
	}
	else if (UnblockType == EUnblockType::Range)
	{	
		Player->UnblockRange();
	}

	if (IsValid(WeaponMesh))
	{
		WeaponMesh->SetHiddenInGame(true);
	}
}