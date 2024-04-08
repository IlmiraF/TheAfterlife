#include "MeleeWeaponItem.h"
#include "../../../Components/WeaponComponents/MeleeHitRegistrator.h"
#include "Engine/DamageEvents.h"
#include "../../../Characters/BaseCharacter.h"

AMeleeWeaponItem::AMeleeWeaponItem()
{
	EquippedSocketName = SocketCharacterWeapon;
}

void AMeleeWeaponItem::StartAttack(EMeleeAttackTypes AttackType)
{
	ABaseCharacter* CharacterOwner = GetCharacterOwner();
	if (!IsValid(CharacterOwner))
	{
		return;
	}

	HitActors.Empty();
	CurrentAttack = Attacks.Find(AttackType);

	if (CurrentAttack && IsValid(CurrentAttack->AttackMontage))
	{
		if (AttackType == EMeleeAttackTypes::HANDS)
		{
			HitRegistrators[0]->AttachToComponent(CachedBaseCharacter->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, "hand_right_collision");
			HitRegistrators[1]->AttachToComponent(CachedBaseCharacter->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, "hand_left_collision");
		}
		else if (AttackType == EMeleeAttackTypes::LEGS)
		{
			HitRegistrators[0]->AttachToComponent(CachedBaseCharacter->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, "foot_right_collision");
			HitRegistrators[1]->AttachToComponent(CachedBaseCharacter->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, "foot_left_collision");
		}

		UAnimInstance* CharacterAnimInstance = CharacterOwner->GetMesh()->GetAnimInstance();
		if (IsValid(CharacterAnimInstance))
		{
			if (CharacterAnimInstance->IsAnyMontagePlaying())
			{
				return;
			}

			int32 RandomIndex = FMath::RandRange(0, CurrentAttack->NameSections.Num() - 1);
			FName RandomElement = CurrentAttack->NameSections[RandomIndex];

			float Duration = CharacterAnimInstance->Montage_Play(CurrentAttack->AttackMontage, 1.0f, EMontagePlayReturnType::Duration);
			CharacterAnimInstance->Montage_JumpToSection(RandomElement, CurrentAttack->AttackMontage);
			bIsAttacking = true;
			GetWorld()->GetTimerManager().SetTimer(AttackTimer, this, &AMeleeWeaponItem::OnAttackTimerElapsed, Duration, false);
		}
		else
		{
			OnAttackTimerElapsed();
		}
	}
}

void AMeleeWeaponItem::SetIsHitRegistrationEnabled(bool bIsRegistrationEnabled)
{
	HitActors.Empty();
	for (UMeleeHitRegistrator* HitRegistrator : HitRegistrators)
	{
		HitRegistrator->SetIsHitRegistrationEnabled(bIsRegistrationEnabled);
		HitRegistrator->SetNotifyRigidBodyCollision(bIsRegistrationEnabled);
	}
}

bool AMeleeWeaponItem::IsAttacking()
{
	return bIsAttacking;
}

void AMeleeWeaponItem::SetCharacter(ABaseCharacter* BaseCharacter)
{
	CachedBaseCharacter = BaseCharacter;
}

void AMeleeWeaponItem::BeginPlay()
{
	Super::BeginPlay();

	GetComponents<UMeleeHitRegistrator>(HitRegistrators);
	for (UMeleeHitRegistrator* HitRegistrator : HitRegistrators)
	{
		HitRegistrator->OnMeleeHitRegistred.AddUFunction(this, FName("ProcessHit"));
	}

}

void AMeleeWeaponItem::ProcessHit(const FHitResult& HitResult, const FVector& HitDirection)
{
	if (CurrentAttack == nullptr)
	{
		return;
	}

	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, TEXT("XYI"));

	AActor* HitActor = HitResult.GetActor();
	if (!IsValid(HitActor))
	{
		return;
	}

	if (HitActors.Contains(HitActor))
	{
		return;
	}

	if (HitActor == GetOwner())
	{
		return;
	}

	FPointDamageEvent DamageEvent;
	DamageEvent.HitInfo = HitResult;
	DamageEvent.ShotDirection = HitDirection;
	DamageEvent.DamageTypeClass = CurrentAttack->DamageTypeClass;

	ABaseCharacter* CharacterOwner = GetCharacterOwner();
	AController* Controller = IsValid(CharacterOwner) ? CharacterOwner->GetController<AController>() : nullptr;
	HitActor->TakeDamage(CurrentAttack->DamageAmount, DamageEvent, Controller, GetOwner());

	HitActors.Add(HitActor);
}

void AMeleeWeaponItem::OnAttackTimerElapsed()
{
	CurrentAttack = nullptr;
	SetIsHitRegistrationEnabled(false);
	bIsAttacking = false;
}
