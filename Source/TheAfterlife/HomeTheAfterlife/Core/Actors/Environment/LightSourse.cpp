
#include "Components/PointLightComponent.h"
#include "NiagaraComponent.h"
#include "LightSourse.h"


ALightSourse::ALightSourse()
{
	PrimaryActorTick.bCanEverTick = true;

	LightSourse = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LightSourse"));
	LightSourse->SetupAttachment(GetRootComponent());

	StylizedFire = CreateDefaultSubobject<UNiagaraComponent>(TEXT("StylizedFire"));
	StylizedFire->SetupAttachment(LightSourse);

	PointLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("PointLight"));
	PointLight->SetupAttachment(LightSourse);

	TargetIntensity = PointLight->Intensity;
}

void ALightSourse::BeginPlay()
{
	Super::BeginPlay();

	PointLight->Intensity = 0;
	StylizedFire->Deactivate();
}
