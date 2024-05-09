#pragma once

#include "CoreMinimal.h"
#include "MovingPlatformComponent.h"
#include "../../../Environment/TriggerButton.h"
#include "LaunchPlatformComponent.generated.h"

UCLASS()
class THEAFTERLIFE_API ULaunchPlatformComponent : public UMovingPlatformComponent
{
	GENERATED_BODY()

public:

	ULaunchPlatformComponent();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Platform movement")
	ATriggerButton* Button;

protected:

	virtual void BeginPlay() override;

private:

	void StartMovingPlatform();
	
};
