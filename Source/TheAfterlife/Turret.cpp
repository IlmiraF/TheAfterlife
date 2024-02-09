#include "Turret.h"
#include "AIController.h"

ATurret::ATurret() 
{
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* TurretRoot = CreateDefaultSubobject<USceneComponent>(TEXT("TurretRoot"));
	SetRootComponent(TurretRoot);

	TurretBaseComponent = CreateDefaultSubobject<USceneComponent>(TEXT("TurretBase"));
	TurretBaseComponent->SetupAttachment(TurretRoot);

	TurretBarrelComponent = CreateDefaultSubobject<USceneComponent>(TEXT("TurretBarrel"));
	TurretBarrelComponent->SetupAttachment(TurretBaseComponent);

	WeaponBarell = CreateDefaultSubobject<UWeaponBarellComponent>(TEXT("Weapon Barrel"));
	WeaponBarell->SetupAttachment(TurretBarrelComponent);
}

void ATurret::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	AAIController* AIController = Cast<AAIController>(NewController);

	if (IsValid(AIController))
	{
		FGenericTeamId TeamId((uint8)Team);
		AIController->SetGenericTeamId(TeamId);
	}
}


void ATurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	switch (CurrentTurretState)
	{
		case SEARCHING:
		{
			SearchingMovement(DeltaTime);
			break;
		}
			
		case FIRING:
		{	
			FiringMovement(DeltaTime);
			break;
		}
			
		default:
			break;
	}
}

void ATurret::SetCurrentTarget(AActor* NewTarget)
{
	CurrentTarget = NewTarget;
	ETurretState NewState = IsValid(CurrentTarget) ? ETurretState::FIRING : ETurretState::SEARCHING;

	SetCurrentTurretState(NewState);
}

FVector ATurret::GetPawnViewLocation() const
{
	return WeaponBarell->GetComponentLocation();
}

FRotator ATurret::GetViewRotation() const
{
	return WeaponBarell->GetComponentRotation();
}

void ATurret::SearchingMovement(float DeltaTime)
{
	FRotator TurretBaseRotation = TurretBaseComponent->GetRelativeRotation();
	TurretBaseRotation.Yaw += DeltaTime * BaseSearchingRotationRate;
	TurretBaseComponent->SetRelativeRotation(TurretBaseRotation);

	FRotator TurretBarrelRotation = TurretBarrelComponent->GetRelativeRotation();
	TurretBarrelRotation.Pitch = FMath::FInterpTo(TurretBarrelRotation.Pitch, 0.0f, DeltaTime, BarellPitchRotationRate);
	TurretBarrelComponent->SetRelativeRotation(TurretBarrelRotation);
}

void ATurret::FiringMovement(float DeltaTime)
{
	FVector BaseLookAtDirection = (CurrentTarget->GetActorLocation() - TurretBaseComponent->GetComponentLocation()).GetSafeNormal2D();
	FQuat LookAtQuat = BaseLookAtDirection.ToOrientationQuat();
	FQuat TargetQuat = FMath::QInterpTo(TurretBaseComponent->GetComponentQuat(), LookAtQuat, DeltaTime, BaseFiringInterpSpeed);
	TurretBaseComponent->SetWorldRotation(TargetQuat);

	FVector BarellLookAtDirection = (CurrentTarget->GetActorLocation() - TurretBarrelComponent->GetComponentLocation()).GetSafeNormal();
	float LookAtPitchAngle = BarellLookAtDirection.ToOrientationRotator().Pitch;

	FRotator BarellLocalRotation = TurretBarrelComponent->GetRelativeRotation();
	BarellLocalRotation.Pitch = FMath::FInterpTo(BarellLocalRotation.Pitch, LookAtPitchAngle, DeltaTime, BarellPitchRotationRate);
	TurretBarrelComponent->SetRelativeRotation(BarellLocalRotation);
}

void ATurret::SetCurrentTurretState(ETurretState NewState)
{
	bool bIsStateChanged = NewState != CurrentTurretState;

	CurrentTurretState = NewState;

	if (!bIsStateChanged)
	{
		return;
	}

	switch (CurrentTurretState)
	{
		case SEARCHING:
		{	
			GetWorld()->GetTimerManager().ClearTimer(ShotTimer);
			break;
		}
		case FIRING:
		{	
			GetWorld() ->GetTimerManager().SetTimer(ShotTimer, this, &ATurret::MakeShot, GetFireInterval(), true, FireDelayTime);
			break;
		}
		default:
			break;
	}
}

float ATurret::GetFireInterval() const
{
	return 60.0f/RateOfFire;
}

void ATurret::MakeShot()
{	
	FVector ShotDirection = WeaponBarell->GetComponentRotation().RotateVector(FVector::ForwardVector);
	float SpreadAngle = FMath::DegreesToRadians(BulletSpreadAngle);
	WeaponBarell->Shot(ShotDirection, SpreadAngle);
}


