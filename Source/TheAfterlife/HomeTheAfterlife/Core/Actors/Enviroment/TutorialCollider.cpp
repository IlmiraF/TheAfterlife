// Fill out your copyright notice in the Description page of Project Settings.


#include "TutorialCollider.h"
#include <TheAfterlife/HomeTheAfterlife/Core/Characters/PlayerCharacter.h>
#include <TheAfterlife/HomeTheAfterlife/Core/Characters/Controllers/BasePlayerController.h>
#include <TheAfterlife/HomeTheAfterlife/Core/Components/CharacterComponents/PlayerUIComponent.h>
#include "EngineUtils.h"
#include "../../../UI\Widget\HintsWidget.h"

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

	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(OtherActor);

	if (!IsValid(PlayerCharacter))
	{
		return;
	}

	UPlayerUIComponent* PlayerUIComponent = PlayerCharacter->GetComponentByClass<UPlayerUIComponent>();

	PlayerUIComponent->GetHintsWidget()->UpdateHint(TutorialText);
	PlayerUIComponent->GetHintsWidget()->UpdateVisible(true);
}

void ATutorialCollider::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{	
	if (!IsValid(OtherActor))
	{
		return;
	}

	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(OtherActor);

	if (!IsValid(PlayerCharacter))
	{
		return;
	}

	UPlayerUIComponent* PlayerUIComponent = PlayerCharacter->GetComponentByClass<UPlayerUIComponent>();

	PlayerUIComponent->GetHintsWidget()->UpdateVisible(false);

	for (TActorIterator<ABird> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		Bird = *ActorItr;
		break;
	}

	if (IsValid(Bird))
	{
		Bird->SetNewPoint(ColliderIndex + 1);
	}
}