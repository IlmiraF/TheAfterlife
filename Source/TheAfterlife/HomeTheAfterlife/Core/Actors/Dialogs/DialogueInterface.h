// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DialogueInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UDialogueInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class THEAFTERLIFE_API IDialogueInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual void StartDialogue() PURE_VIRTUAL(IDialogueInterface::StartDialogue, );
	virtual void FinishDialogue() PURE_VIRTUAL(IDialogueInterface::FinishDialogue, );
	virtual bool GetIsFinished() PURE_VIRTUAL(IDialogueInterface::GetIsFinished, return false;);
};
