#pragma once

#include "CoreMinimal.h"
#include "../../Actors/Environment/Door.h"
#include "../../Characters/Boss/Boss.h"
#include "BossDoor.generated.h"


UCLASS()
class THEAFTERLIFE_API ABossDoor : public ADoor
{
	GENERATED_BODY()

public:

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Boss")
	ABoss* Boss;

private:

	void Open();
};