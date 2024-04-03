#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LightSourse.generated.h"

UCLASS()
class THEAFTERLIFE_API ALightSourse : public AActor
{
	GENERATED_BODY()
	
public:	

	ALightSourse();

	void LightActivate();

protected:

	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UStaticMeshComponent* LightSourse;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UNiagaraComponent* StylizedFire;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UPointLightComponent* PointLight;

private:

	float TargetIntensity = 3000.0f;
};
