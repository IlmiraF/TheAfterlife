#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "../../Actors/Environment/Platforms/PlatformTrigger.h"
#include "TriggerButton.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnButtonActivated);

UCLASS()
class THEAFTERLIFE_API ATriggerButton : public APlatformTrigger
{
	GENERATED_BODY()
	
public:	

	ATriggerButton();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* TriggerMesh;

	FOnButtonActivated OnButtonActivated;

protected:

	virtual void BeginPlay() override;


private:

	UFUNCTION()
	void TakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	bool bButtonIsActive = false;

};
