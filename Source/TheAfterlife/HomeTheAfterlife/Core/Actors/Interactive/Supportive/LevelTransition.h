// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevelTransition.generated.h"

UCLASS()
class THEAFTERLIFE_API ALevelTransition : public AActor
{
	GENERATED_BODY()
	
public:	
	ALevelTransition();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UPrimitiveComponent* InteractionVolume;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	FName LevelName;

	UFUNCTION()
	virtual void OnInteractionVolumeOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:

	bool IsOverlappingCharacterCapsule(AActor* OtherActor, UPrimitiveComponent* OtherComp);

	TWeakObjectPtr<class ABaseCharacter> CachedBaseCharacter;
};
