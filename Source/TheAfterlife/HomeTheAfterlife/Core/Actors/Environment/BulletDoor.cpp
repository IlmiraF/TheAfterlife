#include "BulletDoor.h"

ABulletDoor::ABulletDoor()
{
	PrimaryActorTick.bCanEverTick = true;

	Collider = CreateDefaultSubobject<UBoxComponent>(TEXT("Collider"));
	RootComponent = Collider;

	StoneMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StoneMesh"));
	StoneMesh->SetupAttachment(GetRootComponent());

}

void ABulletDoor::BeginPlay()
{
	Super::BeginPlay();

	OnTakeAnyDamage.AddDynamic(this, &ABulletDoor::TakeAnyDamage);

	//Collider->OnComponentBeginOverlap.AddDynamic(this, &ABulletDoor::OnBulletEnter);
}

void ABulletDoor::OnBulletEnter(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{	
	GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Red, FString::Printf(TEXT("OnBulletEnter")));
	Destroy();
}

void ABulletDoor::TakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{	
	if (DamageType->GetName() != FString("Default__DT_Bullet_C"))
	{
		return;
	}

	Destroy();
}