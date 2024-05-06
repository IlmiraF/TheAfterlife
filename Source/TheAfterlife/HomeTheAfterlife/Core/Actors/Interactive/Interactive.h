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
	virtual void ShouldStopInteracting(bool bStop) PURE_VIRTUAL(IInteractable::ShouldStopInteracting, );
	virtual bool StopInteract() PURE_VIRTUAL(IInteractable::StopInteract, return false;);
	virtual bool HasOnInteractionCallback() const PURE_VIRTUAL(IInteractable::HasOnInteractionCallback, return false; );
	virtual FDelegateHandle AddOnInteractionUFunction(UObject * Object, const FName & FunctionName) PURE_VIRTUAL(IInteractable::AddOnInteractionDelegate, return FDelegateHandle(); );
	virtual void RemoveOnInteractionDelegate(FDelegateHandle DelegateHandle) PURE_VIRTUAL(IInteractable::RemoveOnInteractionDelegate, );
};
