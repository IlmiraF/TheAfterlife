// out your copyright notice in the Description page of Project Settings.


#include "BasePlayerController.h"
#include "../Controllers/BasePlayerController.h"
#include "GameFramework/PlayerInput.h"
#include "../BaseCharacter.h"
#include "../../UI/Widget/PlayerHUDWidget.h"
#include "../../UI/Widget/HintsWidget.h"

void ABasePlayerController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);
	CachedBaseCharacter = Cast<ABaseCharacter>(InPawn);
	if (CachedBaseCharacter.IsValid())
	{
		CreateAndInitializeWidgets();
	}
}

void ABasePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAxis("MoveForward", this, &ABasePlayerController::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &ABasePlayerController::MoveRight);
	InputComponent->BindAxis("Turn", this, &ABasePlayerController::Turn);
	InputComponent->BindAxis("LookUp", this, &ABasePlayerController::LookUp);
	InputComponent->BindAxis("ClimbLadderUp", this, &ABasePlayerController::ClimbLadderUp);
	InputComponent->BindAxis("ClimbMoveForward", this, &ABasePlayerController::ClimbMoveForward);
	InputComponent->BindAxis("ClimbMoveRight", this, &ABasePlayerController::ClimbMoveRight);
	InputComponent->BindAxis("OnBeamMoveForward", this, &ABasePlayerController::OnBeamMoveForward);
	InputComponent->BindAxis("OnBeamMoveRight", this, &ABasePlayerController::OnBeamMoveRight);

	InputComponent->BindAction("InteractWithLadder", EInputEvent::IE_Pressed, this, &ABasePlayerController::InteractWithLadder);
	InputComponent->BindAction("InteractWithRunWall", EInputEvent::IE_Released, this, &ABasePlayerController::InteractWithRunWall);
	InputComponent->BindAction("ClimbHop", EInputEvent::IE_Pressed, this, &ABasePlayerController::ClimbHop);
	InputComponent->BindAction("Climb", EInputEvent::IE_Pressed, this, &ABasePlayerController::OnClimbActionStarted);
	InputComponent->BindAction("Jump", EInputEvent::IE_Pressed, this, &ABasePlayerController::Jump);
	InputComponent->BindAction("Mantle", EInputEvent::IE_Pressed, this, &ABasePlayerController::Mantle);
	InputComponent->BindAction("Crouch", EInputEvent::IE_Pressed, this, &ABasePlayerController::ChangeCrouchState);
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

void ABasePlayerController::InteractWithRunWall()
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->InteractWithRunWall();
	}
}

void ABasePlayerController::ClimbMoveForward(float value)
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->ClimbMoveForward(value);
	}
}

void ABasePlayerController::ClimbMoveRight(float value)
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->ClimbMoveRight(value);
	}
}

void ABasePlayerController::ClimbHop()
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->ClimbHop();
	}
}

void ABasePlayerController::OnClimbActionStarted()
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->OnClimbActionStarted();
	}
}

void ABasePlayerController::OnBeamMoveForward(float value)
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->OnBeamMoveForward(value);
	}
}

void ABasePlayerController::OnBeamMoveRight(float value)
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->OnBeamMoveRight(value);
	}
}

void ABasePlayerController::CreateAndInitializeWidgets()
{
	if (!IsValid(PlayerHUDWidget))
	{
		PlayerHUDWidget = CreateWidget<UPlayerHUDWidget>(GetWorld(), PlayerHUDWidgetClass);

		if (IsValid(PlayerHUDWidget))
		{
			PlayerHUDWidget->AddToViewport();
		}
	}

	if (IsValid(PlayerHUDWidget) && CachedBaseCharacter.IsValid())
	{
		//UHintsWidget* HintsWidget = PlayerHUDWidget->GetHintsWidget();
		//if (IsValid(HintsWidget))
		//{
		//	HintsWidget->UpdateVisible(false);
		//}
	}

	SetInputMode(FInputModeGameOnly{});
	bShowMouseCursor = false;
}

//void ABasePlayerController::UpdateHintsWidget(FString TutorialText, bool Visibility)
//{	
//	if (IsValid(PlayerHUDWidget) && CachedBaseCharacter.IsValid())
//	{	
//		UHintsWidget* HintsWidget = PlayerHUDWidget->GetHintsWidget();
//		if (IsValid(HintsWidget))
//		{	
//			HintsWidget->UpdateHint(TutorialText);
//			HintsWidget->UpdateVisible(Visibility);
//		}
//	}
//}