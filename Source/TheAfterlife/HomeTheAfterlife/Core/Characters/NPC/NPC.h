#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../../Actors/Interfaces/ISpeak.h"
#include "NPC.generated.h"

UCLASS()
class THEAFTERLIFE_API ANPC : public AActor, public IISpeakable
{
	GENERATED_BODY()
	
public:	

	ANPC();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	class USkeletalMeshComponent* NPCMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	UAudioComponent* AudioComponent;

	virtual void Speak(USoundBase* SoundBase) override;
};
