// Copyright Epic Games, Inc. All Rights Reserved.

#include "ProjectAbyssV2GameMode.h"
#include "ProjectAbyssV2Character.h"
#include "UObject/ConstructorHelpers.h"

AProjectAbyssV2GameMode::AProjectAbyssV2GameMode()
{

	numRounds = 3;
	roundTime = 99;
	isTimerActive = false;

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/SideScrollerCPP/Blueprints/SideScrollerCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
