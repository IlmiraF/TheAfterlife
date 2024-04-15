#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "../Interactive/Interactive.h"
#include "../../Actors/Interfaces/ISpeak.h"
#include "DialoguePoint.generated.h"

class UBoxComponent;
class ABaseCharacter;
class UDialogueWidget;
UCLASS()
class THEAFTERLIFE_API ADialoguePoint : public AActor, public IInteractable
{
	GENERATED_BODY()
	
public:	
	ADialoguePoint();

	virtual void Interact(ABaseCharacter* Character) override;
	virtual bool IsForce() override;
	virtual bool HasOnInteractionCallback() const override { return true; }
	virtual FDelegateHandle AddOnInteractionUFunction(UObject* Object, const FName& FunctionName) override;
	virtual void RemoveOnInteractionDelegate(FDelegateHandle DelegateHandle) override;

protected:

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* Collider;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bCanMovePlayer;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsForce = true;

	IInteractable::FOnInteraction OnInteractionEvent;

private:

	bool bItSounded = false;

	void StartAction();
};