#include "NiagaraComponent.h"
#include "Components/PointLightComponent.h"
#include "../../../../TheAfterlifeTypes.h"
#include "LightSourceController.h"

ALightSourceController::ALightSourceController()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TriggerComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerComponent"));
	TriggerComponent->SetupAttachment(GetRootComponent());

	TriggerComponent->SetCollisionProfileName(CollisionProfilePawnInteractionVolume);
	TriggerComponent->SetGenerateOverlapEvents(true);

	TriggerComponent->OnComponentBeginOverlap.AddDynamic(this, &ALightSourceController::OnOverlapBegin);
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