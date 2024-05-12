#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DestroyingStone.generated.h"

UCLASS()
class THEAFTERLIFE_API ADestroyingStone : public AActor
{
	GENERATED_BODY()
	
public:	

	ADestroyingStone();

protected:

	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UStaticMeshComponent* StoneMesh;

private:

	UFUNCTION()
	void TakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

};
