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

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UStaticMeshComponent* Mesh;
};
