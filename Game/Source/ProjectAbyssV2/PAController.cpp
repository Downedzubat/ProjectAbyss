// Fill out your copyright notice in the Description page of Project Settings.


#include "PAController.h"

APAController::APAController()
{

}

void APAController::BeginPlay()
{
	Super::BeginPlay();

	//retrieve character which is being possessed by this controller
	possessedPawn = Cast<AProjectAbyssV2Character>(GetPawn());
	isInputDeviceGamepad = false;
}



void APAController::SetupInputComponent()
{
	Super::SetupInputComponent();

	//set up gameplay inputs
	if (auto gameMode = Cast<AProjectAbyssV2GameMode>(GetWorld()->GetAuthGameMode()))
	{
		if (gameMode->player1 == Cast<AProjectAbyssV2Character>(GetPawn()))
		{
			InputComponent->BindAxis("MoveRight", this, &APAController::CallMoveRight);
		}
		//else
		//{
		//	InputComponent->BindAxis("MoveRightP2", this, &APAController::CallMoveRight);
		//}
	}

	InputComponent->BindAction("P1Jump", IE_Pressed, this, &APAController::CallJump);
	InputComponent->BindAction("P1Jump", IE_Released, this, &APAController::CallStopJumping);
	InputComponent->BindAction("P1Crouch", IE_Pressed, this, &APAController::CallStartCrouching);
	InputComponent->BindAction("P1Crouch", IE_Released, this, &APAController::CallStopCrouching);
	//InputComponent->BindAxis("MoveRightController", this, &APAController::CallMoveRightController);

	//Attack Functions
	InputComponent->BindAction("P1Jab", IE_Pressed, this, &APAController::CallStartJab);
	InputComponent->BindAction("P1Strong", IE_Pressed, this, &APAController::CallStartStrong);
	InputComponent->BindAction("P1Fierce", IE_Pressed, this, &APAController::CallStartFierce);
	InputComponent->BindAction("P1Short", IE_Pressed, this, &APAController::CallStartShort);
	InputComponent->BindAction("P1Long", IE_Pressed, this, &APAController::CallStartLong);
	InputComponent->BindAction("P1Roundhouse", IE_Pressed, this, &APAController::CallStartRoundhouse);
	InputComponent->BindAction("Start", IE_Pressed, this, &APAController::PerformStartInputLogic);
	InputComponent->BindAction("DebugSuper", IE_Pressed, this, &APAController::CallStartTerrorAttack);
	InputComponent->BindAction("Start", IE_Pressed, this, &APAController::DetermineInputDeviceDetails);

	for (int i = 0; i < InputComponent->GetNumActionBindings(); ++i) {
		InputComponent->GetActionBinding(i).bConsumeInput = false;
	}
	
}




void APAController::CallMoveRight(float _value)
{
	if (IsValid(possessedPawn))
	{
		possessedPawn->MoveRight(_value);
	}
}

void APAController::CallJump()
{
	if (IsValid(possessedPawn))
	{
		possessedPawn->Jump();
	}
}

void APAController::CallStopJumping()
{
	if (IsValid(possessedPawn))
	{
		possessedPawn->StopJumping();
	}
}

void APAController::CallStartCrouching()
{
	if (IsValid(possessedPawn))
	{
		possessedPawn->StartCrouching();
	}
}

void APAController::CallStopCrouching()
{
	if (IsValid(possessedPawn))
	{
		possessedPawn->StopCrouching();
	}
}

void APAController::CallStartJab()
{
	if (IsValid(possessedPawn))
	{
		possessedPawn->StartJab();
	}
}

void APAController::CallStartStrong()
{
	if (IsValid(possessedPawn))
	{
		possessedPawn->StartStrong();
	}
}

void APAController::CallStartFierce()
{
	if (IsValid(possessedPawn))
	{
		possessedPawn->StartFierce();
	}
}

void APAController::CallStartShort()
{
	if (IsValid(possessedPawn))
	{
		possessedPawn->StartShort();
	}
}

void APAController::CallStartLong()
{
	if (IsValid(possessedPawn))
	{
		possessedPawn->StartLong();
	}
}

void APAController::CallStartRoundhouse()
{
	if (IsValid(possessedPawn))
	{
		possessedPawn->StartRoundhouse();
	}
}

void APAController::PerformStartInputLogic()
{
	if (auto gameMode = Cast<AProjectAbyssV2GameMode>(GetWorld()->GetAuthGameMode()))
	{
		if (gameMode->matchState == EMatchState::E_Character1Intro || gameMode->matchState == EMatchState::E_Character2Intro)
		{
			if (gameMode->shouldSkipAllCharacterEntrances)
			{
				gameMode->matchState = EMatchState::E_MatchBegin;
			}
			else
			{
				if (gameMode->matchState == EMatchState::E_Character1Intro) {
					gameMode->matchState = EMatchState::E_Character2Intro;
				}
				else if (gameMode->matchState == EMatchState::E_Character2Intro) {
					gameMode->matchState = EMatchState::E_MatchBegin;
				}
			}
		}
		else
		{
			gameMode->PauseGame();
		}
	}
}

void APAController::CallStartTerrorAttack()
{
	if (IsValid(possessedPawn))
	{
		possessedPawn->StartTerrorAttack();
	}
}

void APAController::ClearCharacter()
{
	possessedPawn = nullptr;
}

void APAController::DetermineInputDeviceDetails(FKey _keyPressed)
{
	if (_keyPressed.IsGamepadKey()) {
		isInputDeviceGamepad = true;
	}
	else {
		isInputDeviceGamepad = false;
	}
}

void APAController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!IsValid (possessedPawn))
	{
		possessedPawn = Cast<AProjectAbyssV2Character>(GetPawn());
	}
}
