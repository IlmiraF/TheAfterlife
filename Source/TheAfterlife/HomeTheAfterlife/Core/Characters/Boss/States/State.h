// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "State.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class THEAFTERLIFE_API UState : public UActorComponent
{
	GENERATED_BODY()

public:

	virtual void Enter() {};
	virtual void Exit() {};
		
};
