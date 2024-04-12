// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../../../Environment/AAltar.h"
#include "../../../Environment/Platforms/PlatformComponents/MovingPlatformComponent.h"
#include "FinalPlatformComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class THEAFTERLIFE_API UFinalPlatformComponent : public UMovingPlatformComponent
{
	GENERATED_BODY()

public:	

	UFinalPlatformComponent();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Platform movement")
	AAltar* Altar;

protected:

	virtual void BeginPlay() override;

private:

	void GetDownPlatfrom();
		
};
