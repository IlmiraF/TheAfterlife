#include "AttackStartNotifyState.h"
#include "../../Core\Characters\BaseCharacter.h"
#include "Engine.h"

void UAttackStartNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Green, __FUNCTION__);

	if (MeshComp == NULL || MeshComp->GetOwner() == NULL)
	{	
		return;
	}

	ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(MeshComp->GetOwner());

	if (BaseCharacter != NULL)
	{
		BaseCharacter->MeleeAttackStart();
	}
}

void UAttackStartNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Blue, __FUNCTION__);

	if (MeshComp == NULL || MeshComp->GetOwner() == NULL)
	{
		return;
	}

	ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(MeshComp->GetOwner());

	if (BaseCharacter != NULL)
	{
		BaseCharacter->MeleeAttackFinish();
	}
}
