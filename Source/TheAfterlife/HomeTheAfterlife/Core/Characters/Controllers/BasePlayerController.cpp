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

	InputComponent->BindAction("Jump", EInputEvent::IE_Pressed, this, &ABasePlayerController::Jump);
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

void ABasePlayerController::Jump()
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->Jump();
	}
}
