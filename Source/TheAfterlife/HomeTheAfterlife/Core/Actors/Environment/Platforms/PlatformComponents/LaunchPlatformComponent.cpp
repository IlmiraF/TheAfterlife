#include "LaunchPlatformComponent.h"

ULaunchPlatformComponent::ULaunchPlatformComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void ULaunchPlatformComponent::BeginPlay()
{
	Super::BeginPlay();

	Button->OnButtonActivated.AddUObject(this, &ULaunchPlatformComponent::StartMovingPlatform);
}

void ULaunchPlatformComponent::StartMovingPlatform()
{
	MovePlatform();
}
