
#include "AnimNotify_EnableRagdoll.h"

void UAnimNotify_EnableRagdoll::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	MeshComp->SetCollisionProfileName(FName("Ragdoll"));
	MeshComp->SetSimulatePhysics(true);
}
