#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "WeaponUnblockTrigger.generated.h"

UENUM(BlueprintType)
enum class EUnblockType : uint8
{
	None,
	Range,
	Bomb,
	MAX UMETA(Hidden)
};

UCLASS()
class THEAFTERLIFE_API AWeaponUnblockTrigger : public AActor
{
	GENERATED_BODY()
	
public:	

	AWeaponUnblockTrigger();

protected:
	UFUNCTION()
	void OnPlayerEnter(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	EUnblockType UnblockType;

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* Collider;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UStaticMeshComponent* WeaponMesh;

};
