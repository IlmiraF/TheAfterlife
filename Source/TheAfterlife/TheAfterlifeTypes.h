#pragma once

#define ECC_Climbing ECC_GameTraceChannel1
#define ECC_InteractionVolume ECC_GameTraceChannel2
#define ECC_Bullet ECC_GameTraceChannel3
#define ECC_Melee ECC_GameTraceChannel4

const FName CollisionProfilePawn = FName("Pawn");
const FName CollisionProfileRagdoll = FName("Ragdoll");
const FName CollisionProfilePawnInteractionVolume = FName("PawnInteractionVolume");
const FName SocketCharacterWeapon = FName("CharacterWeaponSocket");

const FName DebugCategoryCharacterAttributes = FName("CharacterAttributes");

UENUM(BlueprintType)
enum class EEquipableItemType : uint8
{
	NONE,
	PISTOL,
	THROWABLE,
	MELEE
};


UENUM(BlueprintType)
enum class EAmunitionType : uint8
{
	NONE,
	PISTOL,
	MAX UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EMeleeAttackTypes : uint8
{
	NONE,
	HANDS,
	LEGS,
	MAX UMETA(Hidden)
};


UENUM(BlueprintType)
enum class EEquipmentSlots : uint8
{
	NONE,
	SIDE_ARM,
	PRIMARY_WEAPON,
	SECONDARY_WEAPON,
	PRIMARY_ITEM_SLOT,
	MELEE_WEAPON,
	MAX UMETA(Hidden)
};

