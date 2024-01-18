// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePlayerController.h"
#include "../Controllers/BasePlayerController.h"
#include "GameFramework/PlayerInput.h"
#include "../BaseCharacter.h"


void ABasePlayerController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);
	CachedBaseCharacter = Cast<ABaseCharacter>(InPawn);
}

void ABasePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAxis("MoveForward", this, &ABasePlayerController::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &ABasePlayerController::MoveRight);
	InputComponent->BindAxis("Turn", this, &ABasePlayerController::Turn);
	InputComponent->BindAxis("LookUp", this, &ABasePlayerController::LookUp);
	InputComponent->BindAxis("ClimbLadderUp", this, &ABasePlayerController::ClimbLadderUp);

	InputComponent->BindAction("InteractWithLadder", EInputEvent::IE_Pressed, this, &ABasePlayerController::InteractWithLadder);
	InputComponent->BindAction("InteractWithZipline", EInputEvent::IE_Released, this, &ABasePlayerController::InteractWithZipline);
	InputComponent->BindAction("Jump", EInputEvent::IE_Pressed, this, &ABasePlayerController::Jump);
	InputComponent->BindAction("Mantle", EInputEvent::IE_Pressed, this, &ABasePlayerController::Mantle);
	InputComponent->BindAction("Crouch", EInputEvent::IE_Pressed, this, &ABasePlayerController::ChangeCrouchState);


	InputComponent->BindAction("Punch", EInputEvent::IE_Pressed, this, &ABasePlayerController::HandsMeleeAttack);
	InputComponent->BindAction("Kick", EInputEvent::IE_Pressed, this, &ABasePlayerController::LegsMeleeAttack);
	InputComponent->BindAction("Fire", EInputEvent::IE_Pressed, this, &ABasePlayerController::Fire);
	InputComponent->BindAction("NextItem", EInputEvent::IE_Pressed, this, &ABasePlayerController::NextItem);
	InputComponent->BindAction("PreviousItem", EInputEvent::IE_Pressed, this, &ABasePlayerController::PreviousItem);
	InputComponent->BindAction("EquipPrimaryItem", EInputEvent::IE_Pressed, this, &ABasePlayerController::EquipPrimaryItem);
}

void ABasePlayerController::MoveForward(float value)
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->MoveForward(value);
	}
}

void ABasePlayerController::MoveRight(float value)
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->MoveRight(value);
	}
}

void ABasePlayerController::Turn(float value)
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->Turn(value);
	}
}

void ABasePlayerController::LookUp(float value)
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->LookUp(value);
	}
}

void ABasePlayerController::Mantle()
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->Mantle();
	}
}

void ABasePlayerController::Jump()
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->Jump();
	}
}

void ABasePlayerController::ChangeCrouchState()
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->ChangeCrouchState();
	}
}

void ABasePlayerController::ClimbLadderUp(float value)
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->ClimbLadderUp(value);
	}
}

void ABasePlayerController::InteractWithLadder()
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->InteractWithLadder();
	}
}

void ABasePlayerController::InteractWithZipline()
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->InteractWithZipline();
	}
}

void ABasePlayerController::NextItem()
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->NextItem();
	}
}

void ABasePlayerController::PreviousItem()
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->PreviousItem();
	}
}

void ABasePlayerController::Fire()
{
	if(CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->Fire();
	}
}

void ABasePlayerController::EquipPrimaryItem()
{
	if (CachedBaseCharacter.IsValid())
	{	
		CachedBaseCharacter->EquipPrimaryItem();
	}
}

void ABasePlayerController::HandsMeleeAttack()
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->HandsMeleeAttack();
	}
}

void ABasePlayerController::LegsMeleeAttack()
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->LegsMeleeAttack();
	}
}
