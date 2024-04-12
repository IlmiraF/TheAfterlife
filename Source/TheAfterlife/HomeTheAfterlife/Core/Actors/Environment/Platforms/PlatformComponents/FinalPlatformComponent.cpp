
#include "FinalPlatformComponent.h"


UFinalPlatformComponent::UFinalPlatformComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UFinalPlatformComponent::BeginPlay()
{
	Super::BeginPlay();
	Altar->OnAltarDestroyed.AddUObject(this, &UFinalPlatformComponent::GetDownPlatfrom);
}


void UFinalPlatformComponent::GetDownPlatfrom()
{
	MovePlatform();
}

