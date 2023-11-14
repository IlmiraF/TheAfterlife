// Copyright Epic Games, Inc. All Rights Reserved.

#include "TheAfterlifeGameMode.h"
#include "TheAfterlifeCharacter.h"
#include "UObject/ConstructorHelpers.h"

ATheAfterlifeGameMode::ATheAfterlifeGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
