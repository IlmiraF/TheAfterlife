// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/SplineComponent.h"
#include "../../Actors/Dialogs/DialoguePoint.h"
#include "../../Actors/Interfaces/ISpeak.h"
#include "../../Actors/Interfaces/ActionDuringSpeech.h"
#include "Bird.generated.h"


UCLASS()
class THEAFTERLIFE_API ABird : public APawn, public IISpeakable, public IActionDuringSpeech
{
	GENERATED_BODY()

public:
	ABird();

	virtual void Tick(float DeltaTime) override;

	void SetNewPoint();

	virtual void Speak(USoundBase* SoundBase) override;

	virtual void ActionDuringSpeech() override;

protected:

	virtual void BeginPlay() override;


	UPROPERTY(EditAnywhere, meta = (MakeEditWidget))
	TArray<ADialoguePoint*> DialoguePointArray;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spline")
	USplineComponent* SplineComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	class USkeletalMeshComponent* BirdMesh;

private:

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	UAudioComponent* BirdAudioComponent;

	UPROPERTY(EditAnywhere)
	TArray<FVector> RouteArray;

	UPROPERTY(EditAnywhere)
	float Speed = 100.0f;

	int32 CurrentIndex = 0;

	void Fly(float DeltaTime);

	float DistanceAlongSpline;

	TArray<float> StopDistances;

	void InitializeSpline();

};