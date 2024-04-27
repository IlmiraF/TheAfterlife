#pragma once

#define ECC_InteractionVolume ECC_GameTraceChannel1
#define ECC_Weapon ECC_GameTraceChannel2
#define ECC_Enemy ECC_GameTraceChannel3
#define ECC_Parkour ECC_GameTraceChannel4
#define ECC_Climbing ECC_GameTraceChannel5
#define ECC_Bullet ECC_GameTraceChannel6
#define ECC_Melee ECC_GameTraceChannel7
#define ECC_Interactive ECC_GameTraceChannel8

const FName CollisionProfilePawn = FName("Pawn");
const FName CollisionProfileRagdoll = FName("Ragdoll");
const FName CollisionProfileNoCollision = FName("NoCollision");
const FName CollisionProfilePawnInteractionVolume = FName("PawnInteractionVolume");
const FName SocketCharacterWeapon = FName("CharacterWeaponSocket");
const FName MeleeCollisionProfileEnabled = FName(TEXT("Weapon"));
const FName NoCollisionProfile = FName(TEXT("NoCollision"));
const FName PickableItemProfile = FName(TEXT("PickableItem"));

const FName DebugCategoryCharacterAttributes = FName("CharacterAttributes");

const FName BB_CurrentTarget = FName("CurrentTarget");
const FName BB_NextLocation = FName("NextLocation");
const FName BB_AltarsDestroyed = FName("AltarsDestroyed");
const FName BB_OnCircleSpline = FName("OnCirleSpline");
const FName BB_BossConcussed = FName("BossConcussed");
const FName BB_OnGround = FName("OnGround");
const FName BB_IsBoy = FName("IsBoy");

const FName HandSocket = FName(TEXT("EquipItemSocket"));


UENUM(BlueprintType)
enum class EEquipableItemType : uint8
{
	NONE,
	SLINGSHOT,
	THROWABLE,
	MELEE
};


UENUM(BlueprintType)
enum class EAmunitionType : uint8
{
	NONE,
	SLINGSHOT,
	MAX UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EMeleeAttackTypes : uint8
{
	NONE,
	HANDS,
	LEGS,
	SWORD,
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

UENUM(BlueprintType)
enum class EReticleType : uint8
{
	None,
	Default,
	SniperRifle,
	MAX UMETA(Hidden)
};

UENUM(BlueprintType)
enum  class ETeams: uint8
{
	PLAYER,
	NPC,
	ENEMY
};

UENUM(BlueprintType)
enum class EBirdFlinghtTypes : uint8
{
	NONE,
	Rise,
	FlyingInCircle,
	MAX UMETA(Hidden)
};

enum class EDamageType : uint8
{
	Bullet,      
	Explosive,   
	Melee,       
	MAX UMETA(Hidden)
};