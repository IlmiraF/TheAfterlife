#include "LightSourceController.h"
#include "Components/BoxComponent.h"
#include "NiagaraComponent.h"
#include "LightSourse.h"
#include "Components/PointLightComponent.h"
#include "../../Characters/BaseCharacter.h"
#include "../../../../TheAfterlifeTypes.h"

ALightSourceController::ALightSourceController()
{
	PrimaryActorTick.bCanEverTick = true;

	TriggerComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerComponent"));
	RootComponent = TriggerComponent;
	TriggerComponent->SetCollisionProfileName(CollisionProfilePawnInteractionVolume);
}

void ALightSourceController::Interact(ABaseCharacter* Character)
{
	if (IsValid(Character))
	{
		TurnOnLight();
	}
}

bool ALightSourceController::IsForce()
{
	return bIsForceIntecation;
}

void ALightSourceController::TurnOnLight()
{
	for (ALightSourse* Light : LightSourses)
	{	
		Light->LightActivate();
	}
}