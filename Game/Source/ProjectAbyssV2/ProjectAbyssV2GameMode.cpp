// Copyright Epic Games, Inc. All Rights Reserved.

#include "ProjectAbyssV2GameMode.h"
#include "ProjectAbyssV2Character.h"
#include "UObject/ConstructorHelpers.h"

AProjectAbyssV2GameMode::AProjectAbyssV2GameMode()
{
	matchState = EMatchState::E_Default;
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

void AProjectAbyssV2GameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//call BP tick function
	BPTick(DeltaTime);

	if (hitstopFrames > 0)
	{
		--hitstopFrames;
		if (hitstopFrames <= 0) 
		{
			if (player1) { player1->EndHitstop(); }
			if (player2) { player2->EndHitstop(); }
		}
			
	}
}

void AProjectAbyssV2GameMode::MatchWon(AProjectAbyssV2Character* _winningCharacter)
{
	TriggerMatchWinEffects(_winningCharacter);
}


