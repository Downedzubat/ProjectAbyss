// Copyright Epic Games, Inc. All Rights Reserved.

#include "ProjectAbyssV2Character.h"
#include "ProjectAbyssV2GameMode.h"
#include "Camera/CameraComponent.h"
#include "MainMenu.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"





AProjectAbyssV2Character::AProjectAbyssV2Character()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Face in the direction we are moving..
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->GravityScale = 2.f;
	GetCharacterMovement()->AirControl = 0.80f;
	GetCharacterMovement()->JumpZVelocity = 1000.f;
	GetCharacterMovement()->GroundFriction = 3.f;
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
	GetCharacterMovement()->MaxFlySpeed = 600.f;

	otherPlayer = nullptr;
	//hurtboxArray* nullptr;


	characterState = ECharacterState::VE_Default;
	comboState = EComboState::E_None;
	attackState = EAttackState::E_None;
	characterClass = ECharacterClass::VE_Default;
	
	scale = FVector(0.0f, 0.0f, 0.0f);
	
	
	wasJabUsed = false;
	wasStrongUsed = false;
	wasFierceUsed = false;
	wasShortUsed = false;
	wasLongUsed = false;
	wasRoundhouseUsed = false;
	wasTerrorAtkUsed = false;
	isFacingRight = false;
	atkHit = false;

	counterHit = false;
	punishCounter = false;

	shouldWallBounce = false;
	shouldGroundBounce = false;
	shouldHardKnockdown = false;
	
	jumpHeight = 1000.0f;
	jumpDistance = 400.0f;
	maxJumpCount = 1;
	jumpCount = 0;
	isPressingBackward = false;

	forwardDashDistance = 800.0f;
	backDashDistance = 600.0f;

	hitstopModifier = 100.0f;

	canFlip = true;
	roundsWon = 0;
	hasLostRound = false;
	isReadyForEntrance = false;
	hasWonMatch = false;

	//hasReleasedAxisInput = true;
	playerHealth = 1.00f;
	maxDistanceApart = 475.0f;
	stunFrames = 0;
	terrorGauge = 0.0f;
	gravityScale = GetCharacterMovement()->GravityScale;
	canMove = true;
	removeInputFromBufferTime = 1.0f;

	comboCounter = 0;

	curTick = 0;
	currentInputsThisFrame = 0;
	capturedInputThisFrame = false;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}



//////////////////////////////////////////////////////////////////////////
// Input

void AProjectAbyssV2Character::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	if (auto gameMode = Cast<AProjectAbyssV2GameMode>(GetWorld()->GetAuthGameMode()))
	{
		//if (gameMode->player1 == this)
		//{
//PlayerInputComponent->BindAxis("MoveRightP1", this, &AProjectAbyssV2Character::MoveRight);
		//}
		//else
		//{
//PlayerInputComponent->BindAxis("MoveRightP2", this, &AProjectAbyssV2Character::MoveRight);
		//}

		NotifyPlayerLockedIn();

		//E_LOG(LogTemp, Warning, TEXT("Player 1 has bound their controls"));

	}
}

void AProjectAbyssV2Character::Jump()
{
	//ACharacter::Jump();
	PerformInputLogic(EInputType::E_Jump, EInputStatus::E_Press);
	if (canMove && characterState != ECharacterState::VE_Crouching && characterState != ECharacterState::VE_CrouchBlocking && comboState == EComboState::E_None && jumpCount)
	{
		//IgnorePlayerToPlayerCollision(true);

		if ((characterState == ECharacterState::VE_MovingBackward && !isFacingRight) || (characterState == ECharacterState::VE_MovingForward && isFacingRight))
		{
			if (characterState == ECharacterState::VE_MovingForward)
			{
				characterState = ECharacterState::VE_ForwardJumping;
			}
			else
			{
				characterState = ECharacterState::VE_BackwardJumping;
			}
			//Use this method for static distances on jump.
			CustomLaunchCharacter(FVector(0.0f, jumpDistance, jumpHeight), true, true);
		}
		else if ((characterState == ECharacterState::VE_MovingForward && isFacingRight) || (characterState == ECharacterState::VE_MovingBackward && !isFacingRight))
		{
			if (characterState == ECharacterState::VE_MovingForward)
			{
				characterState = ECharacterState::VE_ForwardJumping;
			}
			else
			{
				characterState = ECharacterState::VE_BackwardJumping;
			}
			CustomLaunchCharacter(FVector(0.0f, -jumpDistance, jumpHeight), true, true);
		}
		else
		{
			characterState = ECharacterState::VE_NeutralJumping;
			CustomLaunchCharacter(FVector(0.0f, 0.0f, jumpHeight), false, true);
		}


		++jumpCount;

	}
	else if (comboState == EComboState::E_Knockdown)
	{
		comboState = EComboState::E_Recovery;
	}
	
}

void AProjectAbyssV2Character::StopJumping()
{
	ACharacter::StopJumping();
}

void AProjectAbyssV2Character::Landed(const FHitResult& Hit)
{

	GetMovementComponent()->StopMovementImmediately();
	if (characterState == ECharacterState::VE_NeutralJumping || characterState == ECharacterState::VE_ForwardJumping || characterState == ECharacterState::VE_BackwardJumping)
	{

			
			
				if (otherPlayer && Hit.GetActor() == otherPlayer)
				{
					if (otherPlayer->isFacingRight)
					{
						MoveCharacterSmoothly(GetActorLocation(), FVector(GetActorLocation().X, GetActorLocation().Y + (120 - fabs(GetActorLocation().Y - otherPlayer->GetActorLocation().Y)), GetActorLocation().Z - 80.0f));
					}
					else
					{
						MoveCharacterSmoothly(GetActorLocation(), FVector(GetActorLocation().X, GetActorLocation().Y + (120 - fabs(GetActorLocation().Y - otherPlayer->GetActorLocation().Y)) * -1.0f, GetActorLocation().Z - 80.0f));
					}

					GetCharacterMovement()->GravityScale = gravityScale;
				}
				
				
			
				else if (!Cast<AHitboxActor>(Hit.GetActor()))
				{
				GetCharacterMovement()->GravityScale = gravityScale;
				characterState = ECharacterState::VE_Default;
				}
	
	}
	else if ((comboState == EComboState::E_Launched && !shouldGroundBounce) || comboState == EComboState::E_WallBounce || comboState == EComboState::E_FloorBounce)
	{
		if (!Cast<AHitboxActor>(Hit.GetActor()))
		{
			GetCharacterMovement()->GravityScale = gravityScale;

			if (!shouldHardKnockdown)
			{
				comboState = EComboState::E_Knockdown;
			}
			else
			{
				comboState = EComboState::E_HardKnockdown;
				shouldHardKnockdown = false;
			}
			jumpCount = 0;
		}
	}
	else if (shouldGroundBounce)
	{
		comboState = EComboState::E_FloorBounce;
		jumpCount = 0;
	}
	jumpCount = 0;

	IgnorePlayerToPlayerCollision(false);
}

void AProjectAbyssV2Character::CustomLaunchCharacter(FVector _launchVelocity, bool _shouldOverrideXY, bool _shouldOverrideZ, bool _shouldIgnoreCharacterCollision)
{
	if (_shouldIgnoreCharacterCollision)
	{
		IgnorePlayerToPlayerCollision(true);
	}

	LaunchCharacter(_launchVelocity, _shouldOverrideXY, _shouldOverrideZ);
}


void AProjectAbyssV2Character::StartCrouching()
{
	PerformInputLogic(EInputType::E_Crouch, EInputStatus::E_Press);
	//if (canMove && characterState != ECharacterState::VE_LowStunned && characterState != ECharacterState::VE_MidStunned && characterState != ECharacterState::VE_HighStunned && characterState != ECharacterState::VE_Jumping) {
	 if (canMove && comboState == EComboState::E_None)
	 {
		Crouch();

		if (characterState != ECharacterState::VE_Blocking && characterState != ECharacterState::VE_CrouchBlocking)
		{
			characterState = ECharacterState::VE_Crouching;
		}
		else
		{
			characterState = ECharacterState::VE_CrouchBlocking;
		}
		
		canMove = false;
	}

}

void AProjectAbyssV2Character::BeginHitstop(float _damageAmount)
{
	float hitstopTime = _damageAmount * hitstopModifier;
	if (otherPlayer->playerHealth > 0.01f)
	{
		if (hitstopTime > 0)
		{
			if (auto gameMode = Cast<AProjectAbyssV2GameMode>(GetWorld()->GetAuthGameMode()))
			{
				CustomTimeDilation = 0.0f;
				otherPlayer->CustomTimeDilation = 0.0f;
				gameMode->hitstopFrames = hitstopTime;

				gameMode->isTimerActive = true;
			}
		}

	}

}

void AProjectAbyssV2Character::EndCombo()
{
	comboCounter = 0;
}


void AProjectAbyssV2Character::EndHitstop()
{
	CustomTimeDilation = 1.0f;
	otherPlayer->CustomTimeDilation = 1.0f;

	if (auto gameMode = Cast<AProjectAbyssV2GameMode>(GetWorld()->GetAuthGameMode()))
	{
		gameMode->isTimerActive = true;
	}

}

void AProjectAbyssV2Character::StopCrouching()
{
	PerformInputLogic(EInputType::E_Crouch, EInputStatus::E_Release);
	if (comboState == EComboState::E_None)
	{
		UnCrouch();
		if (characterState != ECharacterState::VE_Blocking && characterState != ECharacterState::VE_CrouchBlocking)
		{
			characterState = ECharacterState::VE_Default;
		}
		else
		{
			characterState = ECharacterState::VE_Blocking;
		}

		canMove = true;
	}
}

void AProjectAbyssV2Character::MoveRight(float Value)
{
	//OH GOD
	//Best hope you have an evening free enjoying how I came to this solution
	if (characterState != ECharacterState::VE_Crouching && characterState != ECharacterState::VE_CrouchBlocking)
	{
		if (Value > 0.01f)
		{
			if (!hasReleasedLeftAxisInput)
			{
				hasReleasedLeftAxisInput = true;

				if (isFacingRight)
				{
					PerformInputLogic(EInputType::E_Forward, EInputStatus::E_Release);
				}
				else
				{
					PerformInputLogic(EInputType::E_Backward, EInputStatus::E_Release);
				}
			}

			if (hasReleasedRightAxisInput)
			{
				hasReleasedRightAxisInput = false;

				if (isFacingRight)
				{
					PerformInputLogic(EInputType::E_Backward, EInputStatus::E_Press);
				}
				else
				{
					PerformInputLogic(EInputType::E_Forward, EInputStatus::E_Press);
				}
			}

		}
		else if (Value < -0.01f)
		{
			if (!hasReleasedRightAxisInput)
			{
				hasReleasedRightAxisInput = true;

				if (isFacingRight)
				{
					PerformInputLogic(EInputType::E_Backward, EInputStatus::E_Release);
				}
				else
				{
					PerformInputLogic(EInputType::E_Forward, EInputStatus::E_Release);
				}
			}

			if (hasReleasedLeftAxisInput)
			{
				hasReleasedLeftAxisInput = false;

				if (isFacingRight)
				{
					PerformInputLogic(EInputType::E_Forward, EInputStatus::E_Press);
				}
				else
				{
					PerformInputLogic(EInputType::E_Backward, EInputStatus::E_Press);
				}
			}
		}
	}
	else
	{
		if (Value > 0.01f)
		{
			if (hasReleasedRightAxisInput)
			{
				hasReleasedRightAxisInput = false;

				if (isFacingRight)
				{
					PerformInputLogic(EInputType::E_Forward, EInputStatus::E_Press);
				}
				else
				{
					PerformInputLogic(EInputType::E_Backward, EInputStatus::E_Press);
				}
			}
		}
		else if (Value < -0.01f)
		{
			if (hasReleasedLeftAxisInput)
			{
				hasReleasedLeftAxisInput = false;

				if (isFacingRight)
				{
					PerformInputLogic(EInputType::E_Backward, EInputStatus::E_Press);

				}
				else
				{
					PerformInputLogic(EInputType::E_Forward, EInputStatus::E_Press);
				}
			}
		}
	}
	if (auto MainMenu = Cast<UMainMenu>(GetGameInstance())) 
	{
		if (MainMenu->isDeviceForMultiplePlayers)
		{
			if (canMove && characterState != ECharacterState::VE_Crouching && characterState != ECharacterState::VE_Blocking && characterState != ECharacterState::VE_CrouchBlocking && comboState == EComboState::E_None)
			{
				if (characterState != ECharacterState::VE_NeutralJumping && characterState != ECharacterState::VE_ForwardJumping && characterState != ECharacterState::VE_BackwardJumping && comboState != EComboState::E_Launched)
				{
					if (Value > 0.01f)
					{
						
						hasReleasedRightAxisInput = false;

						if (!isFacingRight)
						{
							characterState = ECharacterState::VE_MovingBackward;
							isPressingBackward = true;
						}
						else
						{
							characterState = ECharacterState::VE_MovingForward;
						}
					}
					else if (Value < -0.01f)
					{
						
						hasReleasedLeftAxisInput = false;
						if (isFacingRight)
						{
							characterState = ECharacterState::VE_MovingBackward;
							isPressingBackward = true;
						}
						else {
							characterState = ECharacterState::VE_MovingForward;
						}
					}
					else
					{
						characterState = ECharacterState::VE_Default;
						//hasReleasedRightAxisInput = true;
						isPressingBackward = false;
					}
				}
			}
			else if (canMove && (characterState == ECharacterState::VE_Crouching || characterState == ECharacterState::VE_CrouchBlocking))
			{
				if (Value > 0.01f)
				{
					hasReleasedRightAxisInput = false;
					if (!isFacingRight)
					{
						isPressingBackward = true;
					}
				}
				else if (Value < -0.01f)
				{
					hasReleasedLeftAxisInput = false;

					if (isFacingRight)
					{
						isPressingBackward = true;
					}
				}
				else
				{
					//hasReleasedAxisInput = true;
					isPressingBackward = false;
				}
			}
			if (otherPlayer) 
			{
				float currentDistanceApart = abs(otherPlayer->GetActorLocation().Y - GetActorLocation().Y);

				if (currentDistanceApart >= maxDistanceApart)
				{
					if ((currentDistanceApart + Value < currentDistanceApart && isFacingRight) || (currentDistanceApart - Value < currentDistanceApart && !isFacingRight))
					{
						// add movement in that direction
						if (canMove) 
						{
							AddMovementInput(FVector(0.f, -1.f, 0.f), Value);
						}

					}
				}
				else
				{
					if (canMove)
					{
						// add movement in that direction
						AddMovementInput(FVector(0.f, -1.f, 0.f), Value);
					}

				}
				
			}
		}
	}

	
}
	


// This function will eventually be un-needed
// But for now, it's here, plaguing this code.
void AProjectAbyssV2Character::MoveRightController(float Value)
{

	if (characterState != ECharacterState::VE_Crouching && characterState != ECharacterState::VE_CrouchBlocking)
	{
		if (Value > 0.01f)
		{
			if (!hasReleasedLeftAxisInput)
			{
				hasReleasedLeftAxisInput = true;

				if (isFacingRight)
				{
					PerformInputLogic(EInputType::E_Forward, EInputStatus::E_Release);
				}
				else
				{
					PerformInputLogic(EInputType::E_Backward, EInputStatus::E_Release);
				}
			}

			if (hasReleasedRightAxisInput)
			{
				hasReleasedRightAxisInput = false;

				if (isFacingRight)
				{
					PerformInputLogic(EInputType::E_Backward, EInputStatus::E_Press);
				}
				else
				{
					PerformInputLogic(EInputType::E_Forward, EInputStatus::E_Press);
				}
			}

		}
		else if (Value < -0.01f)
		{
			if (!hasReleasedRightAxisInput)
			{
				hasReleasedRightAxisInput = true;

				if (isFacingRight)
				{
					PerformInputLogic(EInputType::E_Backward, EInputStatus::E_Release);
				}
				else
				{
					PerformInputLogic(EInputType::E_Forward, EInputStatus::E_Release);
				}
			}

			if (hasReleasedLeftAxisInput)
			{
				hasReleasedLeftAxisInput = false;

				if (isFacingRight)
				{
					PerformInputLogic(EInputType::E_Forward, EInputStatus::E_Press);
				}
				else
				{
					PerformInputLogic(EInputType::E_Backward, EInputStatus::E_Press);
				}
			}
		}
	}
	else
	{
		if (Value > 0.01f)
		{
			if (hasReleasedRightAxisInput)
			{
				hasReleasedRightAxisInput = false;

				if (isFacingRight)
				{
					PerformInputLogic(EInputType::E_Forward, EInputStatus::E_Press);
				}
				else
				{
					PerformInputLogic(EInputType::E_Backward, EInputStatus::E_Press);
				}
			}
		}
		else if (Value < -0.01f)
		{
			if (hasReleasedLeftAxisInput)
			{
				hasReleasedLeftAxisInput = false;

				if (isFacingRight)
				{
					PerformInputLogic(EInputType::E_Backward, EInputStatus::E_Press);

				}
				else
				{
					PerformInputLogic(EInputType::E_Forward, EInputStatus::E_Press);
				}
			}
		}
	}
	if (auto MainMenu = Cast<UMainMenu>(GetGameInstance()))
	{
		if (MainMenu->isDeviceForMultiplePlayers)
		{
			if (canMove && characterState != ECharacterState::VE_Crouching && characterState != ECharacterState::VE_Blocking && characterState != ECharacterState::VE_CrouchBlocking && comboState == EComboState::E_None)
			{
				if (characterState != ECharacterState::VE_NeutralJumping && characterState != ECharacterState::VE_ForwardJumping && characterState != ECharacterState::VE_BackwardJumping && comboState != EComboState::E_Launched)
				{
					if (Value > 0.01f)
					{

						hasReleasedRightAxisInput = false;

						if (!isFacingRight)
						{
							characterState = ECharacterState::VE_MovingBackward;
							isPressingBackward = true;
						}
						else
						{
							characterState = ECharacterState::VE_MovingForward;
						}
					}
					else if (Value < -0.01f)
					{

						hasReleasedLeftAxisInput = false;
						if (isFacingRight)
						{
							characterState = ECharacterState::VE_MovingBackward;
							isPressingBackward = true;
						}
						else {
							characterState = ECharacterState::VE_MovingForward;
						}
					}
					else
					{
						characterState = ECharacterState::VE_Default;
						//hasReleasedRightAxisInput = true;
						isPressingBackward = false;
					}
				}
			}
			else if (canMove && (characterState == ECharacterState::VE_Crouching || characterState == ECharacterState::VE_CrouchBlocking))
			{
				if (Value > 0.01f)
				{
					hasReleasedRightAxisInput = false;
					if (!isFacingRight)
					{
						isPressingBackward = true;
					}
				}
				else if (Value < -0.01f)
				{
					hasReleasedLeftAxisInput = false;

					if (isFacingRight)
					{
						isPressingBackward = true;
					}
				}
				else
				{
					//hasReleasedAxisInput = true;
					isPressingBackward = false;
				}
			}
			if (otherPlayer)
			{
				float currentDistanceApart = abs(otherPlayer->GetActorLocation().Y - GetActorLocation().Y);

				if (currentDistanceApart >= maxDistanceApart)
				{
					if ((currentDistanceApart + Value < currentDistanceApart && isFacingRight) || (currentDistanceApart - Value < currentDistanceApart && !isFacingRight))
					{
						// add movement in that direction
						if (canMove)
						{
							AddMovementInput(FVector(0.f, -1.f, 0.f), Value);
						}

					}
				}
				else
				{
					if (canMove)
					{
						// add movement in that direction
						AddMovementInput(FVector(0.f, -1.f, 0.f), Value);
					}

				}

			}
		}
	}
}


void AProjectAbyssV2Character::TouchStarted(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	// jump on any touch
	Jump();
}

void AProjectAbyssV2Character::TouchStopped(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	StopJumping();
}


void AProjectAbyssV2Character::StartJab()
{
	PerformInputLogic(EInputType::E_Jab, EInputStatus::E_Press);

	if (canAttack && characterState != ECharacterState::VE_Blocking && comboState == EComboState::E_None)
	{
		wasJabUsed = true;
	}
	
}

void AProjectAbyssV2Character::ReleaseJab()
{
	PerformInputLogic(EInputType::E_Jab, EInputStatus::E_Release);
}

void AProjectAbyssV2Character::StartStrong()
{
	
	PerformInputLogic(EInputType::E_Strong, EInputStatus::E_Press);

	if (canAttack && characterState != ECharacterState::VE_Blocking && comboState == EComboState::E_None)
	{
		wasStrongUsed = true;
	}
	
}

void AProjectAbyssV2Character::ReleaseStrong()
{
	PerformInputLogic(EInputType::E_Strong, EInputStatus::E_Release);
}

void AProjectAbyssV2Character::StartFierce()
{
	PerformInputLogic(EInputType::E_Fierce, EInputStatus::E_Press);

	if (canAttack && characterState != ECharacterState::VE_Blocking && comboState == EComboState::E_None)
	{
		wasFierceUsed = true;
	}

}

void AProjectAbyssV2Character::ReleaseFierce()
{
	PerformInputLogic(EInputType::E_Fierce, EInputStatus::E_Release);
}

void AProjectAbyssV2Character::StartShort()
{
	PerformInputLogic(EInputType::E_Short, EInputStatus::E_Press);

	if (canAttack && characterState != ECharacterState::VE_Blocking && comboState == EComboState::E_None)
	{
		wasShortUsed = true;
	}

}

void AProjectAbyssV2Character::ReleaseShort()
{
	PerformInputLogic(EInputType::E_Short, EInputStatus::E_Release);
}

void AProjectAbyssV2Character::StartLong()
{
	PerformInputLogic(EInputType::E_Long, EInputStatus::E_Press);

	if (canAttack && characterState != ECharacterState::VE_Blocking && comboState == EComboState::E_None)
	{
		wasLongUsed = true;
	}

}

void AProjectAbyssV2Character::ReleaseLong()
{
	PerformInputLogic(EInputType::E_Long, EInputStatus::E_Release);
}

void AProjectAbyssV2Character::StartRoundhouse()
{
	PerformInputLogic(EInputType::E_Roundhouse, EInputStatus::E_Press);

	if (canAttack && characterState != ECharacterState::VE_Blocking && comboState == EComboState::E_None)
	{
		wasRoundhouseUsed = true;
	}

}

void AProjectAbyssV2Character::ReleaseRoundhouse()
{
	PerformInputLogic(EInputType::E_Roundhouse, EInputStatus::E_Release);
}

void AProjectAbyssV2Character::StartTerrorAttack()
{
	if (canAttack && comboState == EComboState::E_None)
	{
		if (terrorGauge >= 1.0f)
		{
			wasTerrorAtkUsed = true;
			terrorGauge = 0.0f;
		}
		if (terrorGauge < 0.00f)
		{
			terrorGauge = 0.00f;
		}

	}

}

void AProjectAbyssV2Character::CollidedWithProximityHitbox()
{
	//f ((characterState == ECharacterState::VE_MovingLeft && isFacingRight) || (characterState == ECharacterState::VE_MovingRight && !isFacingRight))
	if (isPressingBackward)
	{
		canMove = false;
		StartProxBlock();
	}
}

void AProjectAbyssV2Character::TakeDamage(float _damageAmount, int _hitstunFrames, int _blockstunFrames, float _launchAmount, float _knockbackAmount, EHitType _hitType, FVector _hitLocation, bool _shouldCauseHardKnockdown)
{
	bool isKOFromHit = false;
	if (!((characterState == ECharacterState::VE_Blocking || characterState == ECharacterState::VE_CrouchBlocking && _hitType == EHitType::E_HIGH || _hitType == EHitType::E_OVERHEAD) ||
		(characterState == ECharacterState::VE_Blocking && _hitType == EHitType::E_MID) ||
		(characterState == ECharacterState::VE_CrouchBlocking && _hitType == EHitType::E_LOW)))
	{
		if (attackState == EAttackState::E_AttackStarted || attackState == EAttackState::E_AttackActive)
		{
			counterHit = true;
			_hitstunFrames += 8;
			playerHealth -= _damageAmount * 1.15;
			UE_LOG(LogTemp, Warning, TEXT("We are countered for %f points"), _damageAmount);
			counterHit = false;
		}
		else if (attackState == EAttackState::E_AttackRecovery)
		{
			punishCounter = true;
			_hitstunFrames += 20;
			playerHealth -= _damageAmount * 1.25;

			UE_LOG(LogTemp, Warning, TEXT("We are punished for %f points"), _damageAmount);
			
			punishCounter = false;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("We are taking damage for %f points"), _damageAmount);
			playerHealth -= _damageAmount;
		}
		
		if (playerHealth < 0.00f)
		{
			isKOFromHit = true;
		}
		
		
		if (stunFrames > 0 || otherPlayer->comboCounter == 0)
		{
			otherPlayer->comboCounter++;
		}

		shouldHardKnockdown = _shouldCauseHardKnockdown;
		//Play VFX and SFX for damage taken
		PlayDamageEffects(_hitLocation, isKOFromHit);
		
		stunFrames = _hitstunFrames;
		
		if (comboState != EComboState::E_Launched && stunFrames > 0)
		{
			switch (_hitType)
			{
			case EHitType::E_OVERHEAD:
				comboState = EComboState::E_HighStunned;
				break;
			case EHitType::E_HIGH:
				comboState = EComboState::E_HighStunned;
				break;
			case EHitType::E_MID:
				comboState = EComboState::E_MidStunned;
				break;
			case EHitType::E_LOW:
				comboState = EComboState::E_LowStunned;
				break;
			case EHitType::E_NONE:
				break;
			}
			PerformKnockback(_knockbackAmount, _launchAmount, false);
			BeginStun();
			BeginHitstop(_damageAmount);
		}
		
		
	

		if (otherPlayer)
		{
			otherPlayer->atkHit = true;
			otherPlayer->terrorGauge += _damageAmount * 0.85f;
		}

		

	
	}
	else
	{
		float reducedDamage = _damageAmount * 0.5f;
		playerHealth -= reducedDamage;
		PlayBlockEffects(_hitLocation, isKOFromHit);
	

		stunFrames = _blockstunFrames;

		if (stunFrames > 0)
		{
			switch (_hitType)
			{
			case EHitType::E_OVERHEAD:
				comboState = EComboState::E_HighStunned;
				break;
			case EHitType::E_HIGH:
				comboState = EComboState::E_HighStunned;
				break;
			case EHitType::E_MID:
				comboState = EComboState::E_MidStunned;
				break;
			case EHitType::E_LOW:
				comboState = EComboState::E_LowStunned;
				break;
			case EHitType::E_NONE:
				break;
			}

			BeginStun();
			PerformKnockback(_knockbackAmount, _launchAmount, true);
			BeginHitstop(_damageAmount);
		 }
		else
		{
			counterHit = false;
			punishCounter = false;
		}
	}
	

	//if (comboState != EComboState::E_Launched && stunFrames > 0)
	//{
	//	comboState = EComboState::E_LowStunned;
	//	BeginStun();
	//}
	//else
	//{
	//	comboState = EComboState::E_Launched;
	//}
	PerformKnockback(_knockbackAmount,  0.0f, false);
	if (playerHealth < 0.00f)
	{
		playerHealth = 0.00f;
		
	}
	if (isKOFromHit)
	{
		if (auto gameMode = Cast<AProjectAbyssV2GameMode>(GetWorld()->GetAuthGameMode()))
		{
			gameMode->isTimerActive = false;
		}
		playerHealth = 0.00f;
		otherPlayer->WinRound();
		NotifyKO();
	}
}

void AProjectAbyssV2Character::PerformKnockback(float _knockbackAmount, float _launchAmount, bool _hasBlocked)
{
	if (characterState ==  ECharacterState::VE_Blocking)
	{
		if (!isFacingRight)
		{
			CustomLaunchCharacter(FVector(0.0f, _knockbackAmount * 2.0f, 0.0f), false, false);
		}
		else
		{
			CustomLaunchCharacter(FVector(0.0f, -_knockbackAmount * 2.0f, 0.0f), false, false);
		}
	}
	else
	{
		if (_launchAmount > 0.0f)
		{
			comboState = EComboState::E_Launched;
			GetCharacterMovement()->GravityScale *= 0.7;
			if (!isFacingRight)
			{
				CustomLaunchCharacter(FVector(0.0f, _knockbackAmount, _launchAmount), false, false);
			}
			else
			{
				CustomLaunchCharacter(FVector(0.0f, -_knockbackAmount, _launchAmount), false, false);
			}

		}
		else {
			if (!isFacingRight)
			{
				CustomLaunchCharacter(FVector(0.0f, _knockbackAmount * 2.0f, 0.0f), false, false);
			}
			else
			{
				CustomLaunchCharacter(FVector(0.0f, -_knockbackAmount * 2.0f, 0.0f), false, false);
			}
		}
	


	
	}
}



void AProjectAbyssV2Character::BeginStun()
{
	if (stunFrames > 0.0f)
	{
		canMove = false;
	}
	
	//GetWorld()->GetTimerManager().SetTimer(stunTimerHandle, this, &AProjectAbyssV2Character::EndStun, stunTime, false);

}

void AProjectAbyssV2Character::EndStun()
{
	if (comboState != EComboState::E_Launched && comboState != EComboState::E_Knockdown && comboState != EComboState::E_Recovery && comboState != EComboState::E_WallBounce && comboState != EComboState::E_FloorBounce)
	{

		if (characterState != ECharacterState::VE_Crouching && characterState != ECharacterState::VE_CrouchBlocking)
		{
			characterState = ECharacterState::VE_Default;
		}
		else
		{
			characterState = ECharacterState::VE_Crouching;
		}
		
		comboState = EComboState::E_None;

		otherPlayer->EndCombo();

	}
	
	canMove = true;
}

void AProjectAbyssV2Character::AddToInputMap(FString _input, EInputType _type)
{
	inputToInputTypeMap.Add(_input, _type);
}

void AProjectAbyssV2Character::AddtoBuffer(FInputInfo _inputInfo)
{
	if (isFacingRight)
	{
		if (_inputInfo.inputType == EInputType::E_Backward)
			_inputInfo.inputType = EInputType::E_Forward;
		else if (_inputInfo.inputType == EInputType::E_Forward)
		{
			_inputInfo.inputType = EInputType::E_Backward;
		}
	}

	if (!capturedInputThisFrame)
	{
		inputBuffer[curTick].inputs[0] = _inputInfo;
		capturedInputThisFrame = true;
	}
	else
	{
		//multiple inputs performed on same frame
		++currentInputsThisFrame;
		inputBuffer[curTick].inputs[currentInputsThisFrame] = _inputInfo;
	}
	
	CheckBufferForCommandType(inputBuffer[curTick].inputs[currentInputsThisFrame].inputStatus);
}




//Check circular buffer for matching command types
//if all conditions within the attack data are met, perform the attack.
void AProjectAbyssV2Character::CheckBufferForCommandType(EInputStatus _inputStatus)
{
	int correctSequenceCounter = 0;

	//for (auto characterCommands[curCommand} : characterCommands)
	for (int curCommand = 0; curCommand < characterCommands.Num(); ++curCommand)
	{
		if (terrorGauge >= characterCommands[curCommand].requiredMeter)
		{
			correctSequenceCounter = characterCommands[curCommand].inputTypes.Num() - 1;

			for (int frame = 0; frame < characterCommands[curCommand].maxFramesBetweenInputs; ++frame)
			{
				int frameDataToCheck = (curTick - frame + inputBuffer.Capacity()) % inputBuffer.Capacity();


				for (int i = 0; i < inputsPerFrame; ++i)
				{
					if (inputBuffer[frameDataToCheck].inputs[i].inputType != EInputType::E_None)
					{
						EInputType type = inputBuffer[frameDataToCheck].inputs[i].inputType;
						EInputStatus status = inputBuffer[frameDataToCheck].inputs[i].inputStatus;
						int64 chargedFrames = inputBuffer[frameDataToCheck].inputs[i].chargedFrames;


						//if a chargeable input gets released
						if (type == characterCommands[curCommand].inputTypes[characterCommands[curCommand].inputTypes.Num() - 1].inputType
							&& _inputStatus == EInputStatus::E_Release && characterCommands[curCommand].isCharging)
						{
							characterCommands[curCommand].isCharging = false;
							characterCommands[curCommand].currHeldFrames = 0;
						}


						if (correctSequenceCounter > -1)
						{
							// Making this as robust as it now is was painful
							if ((type == characterCommands[curCommand].inputTypes[correctSequenceCounter].inputType || (MultiInputCommand(characterCommands[curCommand], type) && status != EInputStatus::E_Release))
								&& (status == characterCommands[curCommand].inputTypes[correctSequenceCounter].inputStatus || (status == EInputStatus::E_Press && characterCommands[curCommand].inputTypes[correctSequenceCounter].inputStatus == EInputStatus::E_Hold))
								&& chargedFrames >= characterCommands[curCommand].inputTypes[correctSequenceCounter].requiredChargeFrames)
							{
								--correctSequenceCounter;
								//inputBuffer[frameDataToCheck].wasUsed = true;
							}
							else if (type != EInputType::E_None && status != EInputStatus::E_Release)
							{
								correctSequenceCounter = characterCommands[curCommand].inputTypes.Num() - 1;
							}
							else if (MultiInputCommand(characterCommands[curCommand], type) && status == EInputStatus::E_Release)
							{
								correctSequenceCounter = characterCommands[curCommand].inputTypes.Num() - 1;
							}
						}
						if (correctSequenceCounter == -1 && _inputStatus != EInputStatus::E_Release)
						{
							if (characterState == characterCommands[curCommand].requiredState)
							{
								moveBuffer.Add(characterCommands[curCommand]);
								break;
							}
							else
							{
								correctSequenceCounter = characterCommands[curCommand].inputTypes.Num() - 1;
							}
						}
					}
					else
					{
						break;
					}

				}

				//prevents commands repeating on release will change if we decide to use negative edge or have a character with a gimmick where a release input is required
				if (correctSequenceCounter == -1 && _inputStatus != EInputStatus::E_Release)
				{
					moveBuffer.Add(characterCommands[curCommand]);
					break;
				}
			}
		}
		
	}
}

bool AProjectAbyssV2Character::MultiInputCommand(FCommand _command, EInputType _pressedInput)
{
	for (auto input : _command.inputTypes)
	{
		if (input.inputType == _pressedInput && input.inputStatus == EInputStatus::E_Hold)
		{
			return true;
		}
	}
	return false;
}

void AProjectAbyssV2Character::ChargeTimeTrackStart(FChargeInputs _inputToTrack)
{
	for (int i = 0; i < chargeTimes.Num(); ++i)
	{
		if (chargeTimes[i].inputType == _inputToTrack.inputType)
		{
			chargeTimes[i].isHoldingInput = true;
		}
	}
}

void AProjectAbyssV2Character::ChargeTimeTrackReset(FChargeInputs _inputToReset)
{
	for (int i = 0; i < chargeTimes.Num(); ++i)
	{
		if (chargeTimes[i].inputType == _inputToReset.inputType)
		{
			chargeTimes[i].isHoldingInput = false;
			chargeTimes[i].chargeFrames = 0;
		}
	}
}



void AProjectAbyssV2Character::DetermineCommandToUse()
{
	if (moveBuffer.Num() > 0)
	{
		FCommand moveToUse = moveBuffer[0];
		for (int i = 1; i < moveBuffer.Num(); ++i)
		{
			if (moveToUse.inputTypes.Num() < moveBuffer[i].inputTypes.Num())
			{
				moveToUse = moveBuffer[i];
			}
		}
		if (attackState == EAttackState::E_None || attackState == EAttackState::E_AttackRecovery)
		{
			StartCommand(moveToUse.name);
		}
		
		moveBuffer.Empty();
	}
}
	

void AProjectAbyssV2Character::StartCommand(FString _commandName)
{
	for (int currentCommand = 0; currentCommand < characterCommands.Num(); ++currentCommand)
	{
		if (_commandName.Compare(characterCommands[currentCommand].name) == 0)
		{
			if (canAttack)
			{
				if (terrorGauge >= characterCommands[currentCommand].requiredMeter)
				{
					terrorGauge -= characterCommands[currentCommand].requiredMeter;

					if (!characterCommands[currentCommand].isCharging && characterCommands[currentCommand].maxHeldFrames > 0)
					{
						//Begin charging a "CHARGE OK" command
						characterCommands[currentCommand].isCharging = true;
					}
					UE_LOG(LogTemp, Warning, TEXT("The character is using the command: %s."), *_commandName);
					characterCommands[currentCommand].hasUsedCommand = true;
				}
			}
		}
	}
}

void AProjectAbyssV2Character::StartProxBlock()
{
	if (canMove && comboState == EComboState::E_None)
	{
		if (characterState != ECharacterState::VE_Crouching && characterState != ECharacterState::VE_CrouchBlocking)
		{
			characterState = ECharacterState::VE_Blocking;
		}
		else
		{
			characterState = ECharacterState::VE_CrouchBlocking;
		}
		
	}
}

void AProjectAbyssV2Character::StopProxBlock()
{
	if (canMove && comboState == EComboState::E_None)
	{
		if (characterState != ECharacterState::VE_Crouching && characterState != ECharacterState::VE_CrouchBlocking)
		{
			characterState = ECharacterState::VE_Default;
		}
		else
		{
			characterState = ECharacterState::VE_Crouching;
		}
	}

}

void AProjectAbyssV2Character::RemovefromBuffer()
{

}

void AProjectAbyssV2Character::RoundWon(AProjectAbyssV2Character* _winningCharacter)
{
	TriggerRoundWinEffects(_winningCharacter);
}


void AProjectAbyssV2Character::Tick(float DeltaTime)
{
	//Disable unneeded logic when character is frozen
	if (CustomTimeDilation > 0.0f)
	{
		Super::Tick(DeltaTime);

		for (int i = 0; i < characterCommands.Num(); ++i)
		{
			//if character is charging attack
			if (characterCommands[i].isCharging)
			{
				if (characterCommands[i].maxHeldFrames > characterCommands[i].currHeldFrames)
				{
					++characterCommands[i].currHeldFrames;
				}
				else
				{
					//perform "release action
					characterCommands[i].isCharging = false;
					characterCommands[i].currHeldFrames = 0;
				}
			}
		}
		//No input given this frame
		if (!capturedInputThisFrame)
		{
			FInputInfo noneInput;
			noneInput.inputType = EInputType::E_None;
			noneInput.frame = GFrameCounter;
			noneInput.chargedFrames = 0;

			for (int i = 0; i < inputsPerFrame; ++i)
			{
				inputBuffer[curTick].inputs[i] = noneInput;
			}

		}
		else
		{
			//reset for next frame
			capturedInputThisFrame = false;
			currentInputsThisFrame = 0;
		}

		//update all charge values
		for (int i = 0; i < chargeTimes.Num(); ++i)
		{
			if (chargeTimes[i].isHoldingInput)
			{
				++chargeTimes[i].chargeFrames;
			}
		}
		if (curTick < 59)
		{
			++curTick;
		}
		else
		{
			curTick = 0;
		}



		//HitStun is now done in frames, not seconds
		if (stunFrames > 0)
		{
			--stunFrames;

			if (stunFrames <= 0)
			{
				EndStun();
			}
		}
		DetermineCommandToUse();

		if (characterState != ECharacterState::VE_NeutralJumping && characterState != ECharacterState::VE_ForwardJumping && characterState != ECharacterState::VE_BackwardJumping && comboState != EComboState::E_WallBounce && canFlip)
		{
			if (otherPlayer)
			{
				if (otherPlayer->GetActorLocation().Y <= GetActorLocation().Y)
				{
					if (isFacingRight)
					{
						GetCapsuleComponent()->GetChildrenComponents(true, capsuleChildren);

						for (auto child : capsuleChildren)
						{
							if (child->GetName().Contains("CharacterMesh"))
							{
								characterMesh = child;
								break;
							}
						}
						if (characterMesh)
						{
							transform = characterMesh->GetRelativeTransform();
							scale = transform.GetScale3D();
							scale.Y = -1.0f;
							scale.X = -1.0f;
							transform.SetScale3D(scale);

							characterMesh->SetRelativeTransform(transform);
						}
						isFacingRight = false;
					}
				}
				else
				{
					if (!isFacingRight)
					{
						GetCapsuleComponent()->GetChildrenComponents(true, capsuleChildren);

						for (auto child : capsuleChildren)
						{
							if (child->GetName().Contains("CharacterMesh"))
							{
								characterMesh = child;
								break;
							}
						}

						transform = characterMesh->GetRelativeTransform();
						scale = transform.GetScale3D();
						scale.Y = 1.0f;
						scale.X = 1.0f;
						transform.SetScale3D(scale);

						characterMesh->SetRelativeTransform(transform);
					}
					isFacingRight = true;
				}
			}
		}
	}	
}

void AProjectAbyssV2Character::WinRound() 
{
	if (!otherPlayer->hasLostRound)
	{
		otherPlayer->hasLostRound = true;
		++roundsWon;

		if (auto gameMode = Cast<AProjectAbyssV2GameMode>(GetWorld()->GetAuthGameMode()))
		{
			if (roundsWon == gameMode->numRounds)
			{
				gameMode->MatchWon(this);
			}
		}
	}
	NotifyRoundEnd();
	UpdateHUDRoundIcons();
}
void AProjectAbyssV2Character::WinMatch()
{
	canMove = false;
	canAttack = false;
	hasWonMatch = true;
}

void AProjectAbyssV2Character::DoubleKO()
{
		otherPlayer->roundsWon++;
		hasLostRound = true;
		otherPlayer->hasLostRound = true;

		NotifyDoubleKO();
		UpdateHUDRoundIcons();
}



	//if (characterState != ECharacterState::VE_Jumping) {


		//if (otherPlayer)
	//	{
	//		if (auto characterMovement = GetCharacterMovement())
	//		{
		//		if (auto enemyMovement = otherPlayer->GetCharacterMovement())
			//	{
				//	if (enemyMovement->GetActorLocation().Y >= characterMovement->GetActorLocation().Y)
					//{
						//// the following code should 110% be using scale.Y, but for some reason with Mask, it has to be scale.Z I'll note this down and look into a fix -FIXED
						//Now when Mask is flipped she is disjointed from hurtbox, I think the best solution is going to be to make our own idle anim using cascadeur
						//if (isFacingRight)
					//	{
						//	if (auto mesh = GetCapsuleComponent()->GetChildComponent(1))
						//	{
//
	//							transform = mesh->GetRelativeTransform();
		//						scale = transform.GetScale3D();
			//					scale.Y = -1;
				//				transform.SetScale3D(scale);
//								mesh->SetRelativeTransform(transform);
	//
		//					}
			//				isFacingRight = false;
		//				}
				//	}
			//		else
	//				{
					//	if (isFacingRight)
		//					if (auto mesh = GetCapsuleComponent()->GetChildComponent(1))
						//	{
				//			//
					//			transform = mesh->GetRelativeTransform();
					//			scale = transform.GetScale3D();
				//				scale.Y = 1;
					//			transform.SetScale3D(scale);
				//				mesh->SetRelativeTransform(transform);

//							}
	//					isFacingRight = true;
			//		}
				//}
			//}
		//}
//	}
//} 