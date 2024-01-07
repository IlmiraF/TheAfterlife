#pragma once

#include "CoreMinimal.h"
#include "../EquipableItem.h"
#include "ThrowableItem.generated.h"


UCLASS(Blueprintable)
class THEAFTERLIFE_API AThrowableItem : public AEquipableItem
{
	GENERATED_BODY()

public:

	void Throw();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Throwables")
	TSubclassOf<class AProjectile> ProjectileClass;
};
