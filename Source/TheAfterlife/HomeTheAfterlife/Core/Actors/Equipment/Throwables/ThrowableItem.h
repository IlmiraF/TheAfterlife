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

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Throwables", meta = (UIMin = -90.0f, UIMax = 90.0f, ClampMin = -90.0f, ClampMax = 90.0f));
	float ThrowAngle = 0.0f;
};
