// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelTransition.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "../../../Characters/BaseCharacter.h"
#include "../../../../../TheAfterlifeTypes.h"
#include "Kismet/GameplayStatics.h"
#include "C:\Program Files\Epic Games\UE_5.2\Engine\Source\Runtime\Engine\Classes\GameFramework\PawnMovementComponent.h"

// Sets default values
ALevelTransition::ALevelTransition()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("VolumeRoot"));

	InteractionVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	InteractionVolume->SetupAttachment(RootComponent);
	InteractionVolume->SetCollisionProfileName(CollisionProfilePawnInteractionVolume);
	InteractionVolume->SetGenerateOverlapEvents(true);
}

void ALevelTransition::BeginPlay()
{
	Super::BeginPlay();
	if (IsValid(InteractionVolume))
	{
		InteractionVolume->OnComponentBeginOverlap.AddDynamic(this, &ALevelTransition::OnInteractionVolumeOverlapBegin);
	}
}

void ALevelTransition::OnInteractionVolumeOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!IsOverlappingCharacterCapsule(OtherActor, OtherComp))
	{
		return;
	}

	//UCharacterMovementComponent* MovementComponent = CachedBaseCharacter->GetMovementComponent();
	//CachedBaseCharacter->GetMovementComponent()->Velocity = FVector::Zero;

	UPrimitiveComponent* RootPlayerComponent = Cast<UPrimitiveComponent>(CachedBaseCharacter->GetRootComponent());
	FVector NewVelocity = FVector::ZeroVector;

	if (RootPlayerComponent != nullptr)
	{
		RootPlayerComponent->SetAllPhysicsLinearVelocity(NewVelocity, false);
	}

	UGameplayStatics::OpenLevel(GetWorld(), LevelName);
}

bool ALevelTransition::IsOverlappingCharacterCapsule(AActor* OtherActor, UPrimitiveComponent* OtherComp)
{
	ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(OtherActor);
	if (!IsValid(BaseCharacter))
	{
		return false;
	}

	CachedBaseCharacter = BaseCharacter;

	if (Cast<UCapsuleComponent>(OtherComp) != BaseCharacter->GetCapsuleComponent())
	{
		return false;
	}
	return true;
}