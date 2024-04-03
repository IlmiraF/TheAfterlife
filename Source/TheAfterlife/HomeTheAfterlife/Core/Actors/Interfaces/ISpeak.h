// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ISpeak.generated.h"

UINTERFACE(MinimalAPI)
class UISpeakable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class THEAFTERLIFE_API IISpeakable
{
	GENERATED_BODY()

public:

	virtual void Speak(USoundBase* SoundBase) PURE_VIRTUAL(IISpeakable::Speak, );
};
