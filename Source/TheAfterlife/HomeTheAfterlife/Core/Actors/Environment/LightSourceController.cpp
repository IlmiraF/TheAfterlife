#include "NiagaraComponent.h"
#include "Components/PointLightComponent.h"
#include "../../../../TheAfterlifeTypes.h"
#include "LightSourceController.h"

ALightSourceController::ALightSourceController()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TriggerComponent1 = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerComponent1"));
	TriggerComponent1->SetupAttachment(GetRootComponent());

	TriggerComponent1->SetCollisionProfileName(CollisionProfilePawnInteractionVolume);
	TriggerComponent1->SetGenerateOverlapEvents(true);

	TriggerComponent1->OnComponentBeginOverlap.AddDynamic(this, &ALightSourceController::OnOverlapBegin);

	TriggerComponent2 = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerComponent2"));
	TriggerComponent2->SetupAttachment(GetRootComponent());

	TriggerComponent2->SetCollisionProfileName(CollisionProfilePawnInteractionVolume);
	TriggerComponent2->SetGenerateOverlapEvents(true);

	TriggerComponent2->OnComponentBeginOverlap.AddDynamic(this, &ALightSourceController::OnOverlapBegin);

	TriggerComponent3 = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerComponent3"));
	TriggerComponent3->SetupAttachment(GetRootComponent());

	TriggerComponent3->SetCollisionProfileName(CollisionProfilePawnInteractionVolume);
	TriggerComponent3->SetGenerateOverlapEvents(true);

	TriggerComponent3->OnComponentBeginOverlap.AddDynamic(this, &ALightSourceController::OnOverlapBegin);

	TriggerComponent4 = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerComponent4"));
	TriggerComponent4->SetupAttachment(GetRootComponent());

	TriggerComponent4->SetCollisionProfileName(CollisionProfilePawnInteractionVolume);
	TriggerComponent4->SetGenerateOverlapEvents(true);

	TriggerComponent4->OnComponentBeginOverlap.AddDynamic(this, &ALightSourceController::OnOverlapBegin);
}

void ALightSourceController::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	TurnOnLight();
}

void ALightSourceController::TurnOnLight()
{
	for (int i = 0; i < LightSourses.Num(); i++)
	{	
		LightSourses[i]->PointLight->SetIntensity(LightSourses[i]->TargetIntensity);
		LightSourses[i]->StylizedFire->Activate();
	}
}