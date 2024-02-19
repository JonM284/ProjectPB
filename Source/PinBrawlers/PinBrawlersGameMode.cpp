// Copyright Epic Games, Inc. All Rights Reserved.

#include "PinBrawlersGameMode.h"
#include "PinBrawlersCharacter.h"
#include "UObject/ConstructorHelpers.h"

APinBrawlersGameMode::APinBrawlersGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
