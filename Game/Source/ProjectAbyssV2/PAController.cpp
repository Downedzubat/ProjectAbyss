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
}

void APAController::SetupInputComponent()
{
	Super::SetupInputComponent();

	//set up gameplay inputs
	if (auto gameMode = Cast<AProjectAbyssV2GameMode>(GetWorld()->GetAuthGameMode()))
	{
		if (gameMode->player1 == Cast<AProjectAbyssV2Character>(GetPawn()))
		{
			InputComponent->BindAxis("MoveRightP1", this, &APAController::CallMoveRight);
		}
		else
		{
			InputComponent->BindAxis("MoveRightP2", this, &APAController::CallMoveRight);
		}
	}

	InputComponent->BindAction("JumpP1", IE_Pressed, this, &APAController::CallJump);
}



void APAController::CallMoveRight(float _value)
{
	if (possessedPawn)
	{
		possessedPawn->MoveRight(_value);
	}
}

void APAController::CallJump()
{
	if (possessedPawn)
	{
		possessedPawn->Jump();
	}
}

void APAController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!possessedPawn)
	{
		possessedPawn = Cast<AProjectAbyssV2Character>(GetPawn());
	}
}
