#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MeshActor.generated.h"

UCLASS()
class THEAFTERLIFE_API AMeshActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AMeshActor();

	AMeshActor* GetMeshActor();

	void SetMesh(UStaticMesh* NewMesh);

	float GetDistanceAlongSpline() const;

	void SetDistanceAlongSpline(float NewDistance);

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UStaticMeshComponent* Mesh;

private:
	float DistanceAlongSpline;
};
