#include "Boss.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "../../Components/CharacterComponents/CharacterEquipmentComponent.h"
#include "../../Actors/Equipment/Weapons/MeleeWeaponItem.h"
#include "../../Actors/Equipment/Weapons/RangeWeaponItem.h"


ABoss::ABoss(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
}

UBehaviorTree* ABoss::GetBehaviorTree() const
{
	return BehaviorTree;
}

void ABoss::SpawnEnemy()
{
	EnemyPoolObject->MakeEnemisVisible();
}

void ABoss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SplineMovement(DeltaTime);
	MoveToBossLocation(DeltaTime);
}

void ABoss::SetSplineMovement(bool Value)
{
	bIsMovingToSpline = Value;
}

void ABoss::SetMovementToGround(bool Value)
{
	bOnGround = !Value;
}

void ABoss::SwitchSplines(EBirdFlinghtTypes FlyType)
{	
	if (FlyType == EBirdFlinghtTypes::FlyingInCircle)
	{
		CachedSplineComponent = CircleSplineActor->GetSplineComponent();
		CurrentFlyType = EBirdFlinghtTypes::FlyingInCircle;
	}
	else
	{
		CachedSplineComponent = RiseSplineActor->GetSplineComponent();
		CurrentFlyType = EBirdFlinghtTypes::Rise;
	}
}

void ABoss::SetInvulnerable(bool Value)
{
	GetCharacterAttributeComponent() ->SetInvulnerable(Value);
}

void ABoss::Concussion()
{
	if (bIsConcussionTimerRunning)
	{
		return;
	}

	bIsConcussionTimerRunning = true;

	GetMesh()->SetSkeletalMesh(SecondStageBossMesh);

	GetCharacterAttributeComponent()->ReduceHealth(ConcussionDamage);

	GetWorld()->GetTimerManager().SetTimer(ConcussionTimerHandle, this, &ABoss::ReturnToBird, TimeConcussion, false);
}

bool ABoss::GetSecondStage()
{
	return bSecondStage;
}

void ABoss::StartSecondStage()
{
	bSecondStage = true;

	GetCharacterAttributeComponent()->RestoreHealth();
}


void ABoss::BeginPlay()
{
	Super::BeginPlay();;

	GetMesh()->SetSkeletalMesh(SecondStageBossMesh);

	for (AAltar* Altar : Altars)
	{
		Altar->OnAltarDestroyed.AddUObject(this, &ABoss::DestructionAltars);
	}

	AmountAltars = Altars.Num();

	GetCharacterAttributeComponent()->OnHealthChangedEvent.AddUObject(this, &ABoss::ChangeHealth);
}

void ABoss::DestructionAltars()
{
	AmountAltars--;

	BossConcussed();
	

	if (AmountAltars<=0)
	{
		FirstStageCompleted();
	}
}

void ABoss::BossConcussed()
{	
	bOnConcussed = true;
	bIsMovingToSpline = false;

	if (OnBossConcussed.IsBound())
	{	
		OnBossConcussed.Broadcast(bOnConcussed);
	}
}

void ABoss::ReturnToBird()
{	
	DistanceAlongSpline = 0;
	bIsConcussionTimerRunning = false;

	bOnConcussed = false;
	bOnGround = false;

	if (OnMovedToCircleSpline.IsBound())
	{
		OnMovedToCircleSpline.Broadcast(false);
	}
	
	if (OnBossConcussed.IsBound())
	{
		OnBossConcussed.Broadcast(bOnConcussed);
	}

	if (OnBossHasLanded.IsBound())
	{
		OnBossHasLanded.Broadcast(bOnGround);
	}

	GetMesh()->SetSkeletalMesh(FirstStageBossMesh);
	CurrentFlyType = EBirdFlinghtTypes::Rise;
}

void ABoss::ChangeHealth(float newHealthPercent)
{	
	if (newHealthPercent <= 66.6 && newHealthPercent > 33.3 && SecondPhase == false)
	{
		SecondPhase = true;
		NewPhase();
	}
	else if (newHealthPercent <= 33.3 && newHealthPercent > 0 && ThirdPhase == false)
	{	
		ThirdPhase = true;
		NewPhase();
	}
}

void ABoss::NewPhase()
{
	if (OnBossIsBoy.IsBound())
	{
		OnBossIsBoy.Broadcast(false);
	}

	BoosterSelection();
}

void ABoss::BoosterSelection()
{
	EDamageType DamageType = GetCharacterAttributeComponent()->GetMostDamagingType();	//ÄÎ ÑÞÄÀ ÂÑ¨ ÐÀÁÎÒÀÅÒ, ÍÀÑÒÐÎÈÒÜ ÓÑÈËÈÒÅËÈ
	
	
	if (DamageType == EDamageType::Bullet)
	{	
		GetCharacterEquipmentComponent()->GetCurrentMeleeWeaponItem()->SetDefaultBoosterDamage();
		GetCharacterEquipmentComponent()->GetCurrentRangeWeapon()->SetWeaponBooster(BoosterBulletDamage, ShootingAccuracyBooster);
	}
	else if(DamageType == EDamageType::Explosive)
	{	
		//GetCharacterEquipmentComponent()->GetCurrentMeleeWeaponItem()->SetDefaultBoosterDamage();
	}
	else if (DamageType == EDamageType::Melee)
	{
		GetCharacterEquipmentComponent()->GetCurrentRangeWeapon()->SetDefaultWeaponBooster();
		GetCharacterEquipmentComponent()->GetCurrentMeleeWeaponItem()->SetBoosterDamage(BoosterMeleeDamage);
	}
	
	GetCharacterAttributeComponent()->ClearDamageCounters();
}

void ABoss::FirstStageCompleted()
{
	if (OnFirstStageCompleted.IsBound())
	{
		OnFirstStageCompleted.Broadcast();
	}
}

void ABoss::SplineMovement(float DeltaTime)
{	
	if (!bIsMovingToSpline)
	{
		return;
	}

	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
	
	float NewDistance = DistanceAlongSpline + FlySpeed * DeltaTime;

	FVector NewLocation = CachedSplineComponent->GetLocationAtDistanceAlongSpline(NewDistance, ESplineCoordinateSpace::World);
	FRotator NewRotation = CachedSplineComponent->GetRotationAtDistanceAlongSpline(NewDistance, ESplineCoordinateSpace::World);

	SetActorLocationAndRotation(NewLocation, NewRotation);

	DistanceAlongSpline = NewDistance;
	float SplineLength = GetSplineLength();

	if (DistanceAlongSpline >= SplineLength)
	{
		MovementAlongCompleted();
	}
}

void ABoss::MovementAlongCompleted()
{
	if (CurrentFlyType == EBirdFlinghtTypes::FlyingInCircle)
	{
		DistanceAlongSpline = 0;
	}
	else if (CurrentFlyType == EBirdFlinghtTypes::Rise)
	{	
		if (OnMovedToCircleSpline.IsBound())
		{
			OnMovedToCircleSpline.Broadcast(true);
		}
	}
}

float ABoss::GetSplineLength()
{
	float Length = 0.0f;
	int32 NumPoints = CachedSplineComponent->GetNumberOfSplinePoints();

	for (int32 i = 1; i < NumPoints; ++i)
	{
		FVector PrevPoint = CachedSplineComponent->GetLocationAtSplinePoint(i - 1, ESplineCoordinateSpace::World);
		FVector CurPoint = CachedSplineComponent->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::World);
		Length += FVector::Distance(PrevPoint, CurPoint);
	}

	return Length;
}


void ABoss::MoveToBossLocation(float DeltaTime)
{	
	if (!bOnConcussed)
	{
		return;
	}

	if (bOnGround)
	{
		return;
	}

	if (!GetActorLocation().Equals(BossLocation, 10.0f))
	{
		FVector Direction = (BossLocation - GetActorLocation()).GetSafeNormal();

		FVector NewLocation = GetActorLocation() + (Direction * FlySpeed * DeltaTime);
		FRotator NewRotation = Direction.Rotation();

		SetActorLocationAndRotation(NewLocation, NewRotation);
	}
	else
	{	
		bOnGround = true;

		if (OnBossHasLanded.IsBound())
		{
			OnBossHasLanded.Broadcast(bOnGround);
		}
	}
}