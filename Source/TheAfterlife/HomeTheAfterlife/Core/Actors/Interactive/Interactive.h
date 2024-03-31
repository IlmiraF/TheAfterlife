// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interactive.generated.h"

/**
 * 
 */
class ABaseCharacter;
UINTERFACE(MinimalAPI)
class UInteractable : public UInterface 
{
	GENERATED_BODY()
};

class THEAFTERLIFE_API IInteractable
{

	GENERATED_BODY()

public:

	DECLARE_MULTICAST_DELEGATE(FOnInteraction);

	virtual void Interact(ABaseCharacter* Character) PURE_VIRTUAL(IInteractable::Interact, );
	virtual bool IsForce() PURE_VIRTUAL(IInteractable::IsForce, return false;);
};
