
#include "AnimNotify_EnableRagdoll.h"
#include "../../../../../TheAfterlifeTypes.h"

void UAnimNotify_EnableRagdoll::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	MeshComp->SetCollisionProfileName(CollisionProfileRagdoll);
	MeshComp->SetSimulatePhysics(true);
}
