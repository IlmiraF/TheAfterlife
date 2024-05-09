#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "TriggerButton.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnButtonActivated);

UCLASS()
class THEAFTERLIFE_API ATriggerButton : public AActor
{
	GENERATED_BODY()
	
public:	

	ATriggerButton();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	UBoxComponent* TriggerComponent;

	FOnButtonActivated OnButtonActivated;

private:

	UFUNCTION()
	void TakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	bool bButtonIsActive = false;

};
