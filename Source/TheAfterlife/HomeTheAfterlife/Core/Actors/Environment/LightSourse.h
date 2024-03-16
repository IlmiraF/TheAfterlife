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

	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UStaticMeshComponent* LightSourse;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UNiagaraComponent* StylizedFire;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UPointLightComponent* PointLight;

	float TargetIntensity = 2000.0f;

protected:

	virtual void BeginPlay() override;


};
