// Fill out your copyright notice in the Description page of Project Settings.


#include "TutorialCollider.h"
#include "Components/BoxComponent.h"
#include "../../../Characters/BaseCharacter.h"

// Sets default values
ATutorialCollider::ATutorialCollider()
{
	PrimaryActorTick.bCanEverTick = true;
	Collider = CreateDefaultSubobject<UBoxComponent>(TEXT("Collider"));
	RootComponent = Collider;

	Collider->OnComponentBeginOverlap.AddDynamic(this, &ATutorialCollider::OnOverlapBegin);
	Collider->OnComponentEndOverlap.AddDynamic(this, &ATutorialCollider::OnOverlapEnd);
}

void ATutorialCollider::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!IsValid(OtherActor))
	{
		return;
	}

	ABaseCharacter* PlayerCharacter = Cast<ABaseCharacter>(OtherActor);

	if (IsValid(PlayerCharacter))
	{
		OnReachedTarget();
		PlayerCharacter->OnTutorialColliderOverlapped.ExecuteIfBound(TutorialText, true);
	}
}

void ATutorialCollider::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!IsValid(OtherActor))
	{
		return;
	}

	ABaseCharacter* PlayerCharacter = Cast<ABaseCharacter>(OtherActor);

	if (IsValid(PlayerCharacter))
	{
		PlayerCharacter->OnTutorialColliderOverlapped.ExecuteIfBound("", false);
	}
}

void ATutorialCollider::OnReachedTarget()
{
	if (OnReachedTargetEvent.IsBound())
	{
		OnReachedTargetEvent.Broadcast();
	}
}
