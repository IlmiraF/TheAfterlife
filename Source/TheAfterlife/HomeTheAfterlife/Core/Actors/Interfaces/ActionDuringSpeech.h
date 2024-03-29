// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ActionDuringSpeech.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UActionDuringSpeech : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class THEAFTERLIFE_API IActionDuringSpeech
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual void ActionDuringSpeech() {};
};
