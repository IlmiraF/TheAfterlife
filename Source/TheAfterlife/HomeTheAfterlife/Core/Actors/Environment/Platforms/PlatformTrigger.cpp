// Fill out your copyright notice in the Description page of Project Settings.


#include "PlatformTrigger.h"
#include "../../../../../TheAfterlifeTypes.h"
#include "Components/BoxComponent.h"
#include "../../../Characters/BaseCharacter.h"

APlatformTrigger::APlatformTrigger()
{
	/*bReplicates = true;
	NetUpdateFrequency = 2.0f;
	MinNetUpdateFrequency = 2.0f;*/
	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	TriggerBox->SetCollisionProfileName(CollisionProfilePawnInteractionVolume);
	SetRootComponent(TriggerBox);
}

void APlatformTrigger::BeginPlay()
{
	Super::BeginPlay();
	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &APlatformTrigger::OnTriggerOverlapBegin);
	TriggerBox->OnComponentEndOverlap.AddDynamic(this, &APlatformTrigger::OnTriggerOverlapEnd);
}

void APlatformTrigger::SetIsActivated(bool bIsActivated_In)
{
	if (OnTriggerActivated.IsBound())
	{
		OnTriggerActivated.Broadcast(bIsActivated_In);
	}
}

void APlatformTrigger::OnTriggerOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ABaseCharacter* OtherPawn = Cast<ABaseCharacter>(OtherActor);
	if (!IsValid(OtherPawn))
	{
		return;
	}

	OverlappedPawns.AddUnique(OtherPawn);
	GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Red, FString::Printf(TEXT("OnTriggerOverlapBegin")));
	if (!bIsActivated && OverlappedPawns.Num() > 0)
	{	
		GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Green, FString::Printf(TEXT("OnTriggerOverlapBegin")));
		SetIsActivated(true);
		bIsActivated = true;
	}
}

void APlatformTrigger::OnTriggerOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ABaseCharacter* OtherPawn = Cast<ABaseCharacter>(OtherActor);
	if (!IsValid(OtherPawn))
	{
		return;
	}

	OverlappedPawns.RemoveSingleSwap(OtherPawn);
	SetIsActivated(false);
	bIsActivated = false;
}