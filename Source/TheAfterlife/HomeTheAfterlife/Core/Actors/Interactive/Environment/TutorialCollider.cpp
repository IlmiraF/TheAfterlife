// Fill out your copyright notice in the Description page of Project Settings.


#include "TutorialCollider.h"
#include "../../../Characters/Birds/Bird.h"
#include "../../../Characters/PlayerCharacter.h"
#include "../../../Characters/Controllers/BasePlayerController.h"
#include "EngineUtils.h"
#include "../../../UI/Widget/HintsWidget.h"

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

	ABasePlayerController* BasePlayerController = Cast<ABasePlayerController>(PlayerCharacter->GetController());

	if (IsValid(BasePlayerController))
	{
		BasePlayerController->UpdateHintsWidget(TutorialText, true);
	}
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

	ABasePlayerController* BasePlayerController = Cast<ABasePlayerController>(PlayerCharacter->GetController());

	if (IsValid(BasePlayerController))
	{
		BasePlayerController->UpdateHintsWidget(TutorialText, false);
	}

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