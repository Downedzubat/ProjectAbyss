// Copyright Epic Games, Inc. All Rights Reserved.

#include "ProjectAbyssV2Character.h"
#include "ProjectAbyssV2GameMode.h"
#include "Camera/CameraComponent.h"
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

	/** Create a camera boom attached to the root(capsule)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Rotation of the character should not affect rotation of boom
	CameraBoom->bDoCollisionTest = false;
	CameraBoom->TargetArmLength = 500.f;
	CameraBoom->SocketOffset = FVector(0.f,0.f,75.f);
	CameraBoom->SetRelativeRotation(FRotator(0.f,180.f,0.f));

	// Create a camera and attach to boom
	SideViewCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("SideViewCamera"));
	SideViewCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	SideViewCameraComponent->bUsePawnControlRotation = false; // We don't want the controller rotating the camera
	*/

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
	hurtbox = nullptr;
	scale = FVector(0.0f, 0.0f, 0.0f);
	wasJabUsed = false;
	wasStrongUsed = false;
	wasFierceUsed = false;
	wasShortUsed = false;
	wasLongUsed = false;
	wasRoundhouseUsed = false;
	wasTerrorAtkUsed = false;
	isFlipped = false;
	atkHit = false;
	

	hasReleasedAxisInput = true;
	playerHealth = 1.00f;
	maxDistanceApart = 600.0f;
	stunTime = 0.0f;
	terrorGauge = 0.0f;
	gravityScale = GetCharacterMovement()->GravityScale;
	canMove = true;
	removeInputFromBufferTime = 1.0f;

	characterCommands.SetNum(3);

	characterCommands[0].name = "Monstrous Swing";
	characterCommands[0].inputTypes.Add(EInputType::E_Crouch);
	characterCommands[0].inputTypes.Add(EInputType::E_Forward);
	characterCommands[0].inputTypes.Add(EInputType::E_Jab);
	characterCommands[0].hasUsedCommand = false;

	characterCommands[1].name = "Warrior Spirit";
	characterCommands[1].inputTypes.Add(EInputType::E_Crouch);
	characterCommands[1].inputTypes.Add(EInputType::E_Backward);
	characterCommands[1].inputTypes.Add(EInputType::E_Strong);
	characterCommands[1].hasUsedCommand = false;

	characterCommands[2].name = "Super";
	characterCommands[2].inputTypes.Add(EInputType::E_Crouch);
	characterCommands[2].inputTypes.Add(EInputType::E_Forward);
	characterCommands[2].inputTypes.Add(EInputType::E_Crouch);
	characterCommands[2].inputTypes.Add(EInputType::E_Forward);
	characterCommands[2].inputTypes.Add(EInputType::E_Fierce);
	characterCommands[2].hasUsedSuper = false;
	characterCommands[2].hasUsedCommand = false;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}



//////////////////////////////////////////////////////////////////////////
// Input

void AProjectAbyssV2Character::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	if  (auto gameMode = Cast<AProjectAbyssV2GameMode>(GetWorld()->GetAuthGameMode()))
	{
		if (gameMode->player1 == this)
		{
			UE_LOG(LogTemp, Warning, TEXT("Player 1 has bound their controls"));
			// set up gameplay key bindings
			PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AProjectAbyssV2Character::Jump);
			PlayerInputComponent->BindAction("Jump", IE_Released, this, &AProjectAbyssV2Character::StopJumping);
			PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &AProjectAbyssV2Character::StartCrouching);
			PlayerInputComponent->BindAction("Crouch", IE_Released, this, &AProjectAbyssV2Character::StopCrouching);
			PlayerInputComponent->BindAxis("MoveRight", this, &AProjectAbyssV2Character::MoveRight);

			PlayerInputComponent->BindTouch(IE_Pressed, this, &AProjectAbyssV2Character::TouchStarted);
			PlayerInputComponent->BindTouch(IE_Released, this, &AProjectAbyssV2Character::TouchStopped);

			//Attack Functions
			PlayerInputComponent->BindAction("Jab", IE_Pressed, this, &AProjectAbyssV2Character::StartJab);

			PlayerInputComponent->BindAction("Strong", IE_Pressed, this, &AProjectAbyssV2Character::StartStrong);

			PlayerInputComponent->BindAction("Fierce", IE_Pressed, this, &AProjectAbyssV2Character::StartFierce);

			PlayerInputComponent->BindAction("Short", IE_Pressed, this, &AProjectAbyssV2Character::StartShort);

			PlayerInputComponent->BindAction("Long", IE_Pressed, this, &AProjectAbyssV2Character::StartLong);

			PlayerInputComponent->BindAction("Roundhouse", IE_Pressed, this, &AProjectAbyssV2Character::StartRoundhouse);
		}
		else
		{
			//PLAYER 2 STUFF
			// set up gameplay key bindings
			PlayerInputComponent->BindAction("JumpP2", IE_Pressed, this, &AProjectAbyssV2Character::Jump);
			PlayerInputComponent->BindAction("JumpP2", IE_Released, this, &AProjectAbyssV2Character::StopJumping);
			PlayerInputComponent->BindAction("CrouchP2", IE_Pressed, this, &AProjectAbyssV2Character::StartCrouching);
			PlayerInputComponent->BindAction("CrouchP2", IE_Released, this, &AProjectAbyssV2Character::StopCrouching);
			PlayerInputComponent->BindAxis("MoveRightP2", this, &AProjectAbyssV2Character::MoveRight);

			PlayerInputComponent->BindTouch(IE_Pressed, this, &AProjectAbyssV2Character::TouchStarted);
			PlayerInputComponent->BindTouch(IE_Released, this, &AProjectAbyssV2Character::TouchStopped);

			//Attack Functions
			PlayerInputComponent->BindAction("JabP2", IE_Pressed, this, &AProjectAbyssV2Character::StartJab);

			PlayerInputComponent->BindAction("StrongP2", IE_Pressed, this, &AProjectAbyssV2Character::StartStrong);

			PlayerInputComponent->BindAction("FierceP2", IE_Pressed, this, &AProjectAbyssV2Character::StartFierce);

			PlayerInputComponent->BindAction("ShortP2", IE_Pressed, this, &AProjectAbyssV2Character::StartShort);

			PlayerInputComponent->BindAction("LongP2", IE_Pressed, this, &AProjectAbyssV2Character::StartLong);

			PlayerInputComponent->BindAction("RoundhouseP2", IE_Pressed, this, &AProjectAbyssV2Character::StartRoundhouse);
		}
	}
}

void AProjectAbyssV2Character::Jump()
{
	ACharacter::Jump();
	characterState = ECharacterState::VE_Jumping;
}

void AProjectAbyssV2Character::StopJumping()
{
	ACharacter::StopJumping();
}

void AProjectAbyssV2Character::Landed(const FHitResult& Hit)
{
	if (characterState == ECharacterState::VE_Launched || characterState == ECharacterState::VE_Jumping)
	{
		ACharacter::Landed(Hit);
		GetCharacterMovement()->GravityScale = gravityScale;
		characterState = ECharacterState::VE_Default;
	}
}

void AProjectAbyssV2Character::StartCrouching()
{
	characterState = ECharacterState::VE_Crouching;
	isCrouching = true;
	canMove = false;
}

void AProjectAbyssV2Character::StopCrouching()
{
	characterState = ECharacterState::VE_Default;
	isCrouching = false;
	canMove = true;
}

void AProjectAbyssV2Character::MoveRight(float Value)
{

	if (canMove && characterState != ECharacterState::VE_Crouching && characterState != ECharacterState::VE_Blocking)
	{
		if (characterState != ECharacterState::VE_Jumping && characterState != ECharacterState::VE_Launched)
		{
			if (Value > 0.20f)
			{
				characterState = ECharacterState::VE_MovingRight;
				hasReleasedAxisInput = false;
			}
			else if (Value < -0.20f)
			{
				characterState = ECharacterState::VE_MovingLeft;
				hasReleasedAxisInput = false;
			}
			else
			{
				characterState = ECharacterState::VE_Default;
				hasReleasedAxisInput = true;
			}
		}
	}

	float currentDistanceApart = abs(otherPlayer->GetActorLocation().Y - GetActorLocation().Y);

	if (currentDistanceApart >= maxDistanceApart)
	{
		if ((currentDistanceApart + Value < currentDistanceApart && !isFlipped) || (currentDistanceApart - Value < currentDistanceApart && isFlipped))
		{
			// add movement in that direction
			if (canMove) {
				AddMovementInput(FVector(0.f, -1.f, 0.f), Value);
			}
			
		}
	}
	else
	{
		if (canMove) {
			// add movement in that direction
			AddMovementInput(FVector(0.f, -1.f, 0.f), Value);
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
	UE_LOG(LogTemp, Warning, TEXT("We are using JAB"))
	wasJabUsed = true;
	canMove = false;
}

void AProjectAbyssV2Character::StartStrong()
{
	UE_LOG(LogTemp, Warning, TEXT("We are using STRONG"))
	wasStrongUsed = true;
	canMove = false;
}

void AProjectAbyssV2Character::StartFierce()
{
	UE_LOG(LogTemp, Warning, TEXT("We are using FIERCE"))
	wasFierceUsed = true;
	canMove = false;
}

void AProjectAbyssV2Character::StartShort()
{
	UE_LOG(LogTemp, Warning, TEXT("We are using SHORT"))
	wasShortUsed = true;
	canMove = false;
}

void AProjectAbyssV2Character::StartLong()
{
	UE_LOG(LogTemp, Warning, TEXT("We are using LONG"))
	wasLongUsed = true;
	canMove = false;
}

void AProjectAbyssV2Character::StartRoundhouse()
{
	UE_LOG(LogTemp, Warning, TEXT("We are using ROUNDHOUSE"))
	wasRoundhouseUsed = true;
	canMove = false;
}

void AProjectAbyssV2Character::StartTerrorAttack()
{

	if (terrorGauge > 1.0f)
	{
		wasTerrorAtkUsed = true;
	}
	if (terrorGauge < 0.00f)
	{
		terrorGauge = 0.00f;
	}
}

void AProjectAbyssV2Character::CollidedWithProximityHitbox()
{
	if ((characterState == ECharacterState::VE_MovingLeft && isFlipped) || (characterState == ECharacterState::VE_MovingRight && !isFlipped))
	{
		canMove = false;
		UE_LOG(LogTemp, Warning, TEXT("The Character is autoblocking."));
		characterState = ECharacterState::VE_Blocking;
	}
}

void AProjectAbyssV2Character::TakeDamage(float _damageAmount, float _stunTime, float _blockstunTime, float _launchAmount, float _knockbackAmount)
{
	if (characterState != ECharacterState::VE_Blocking) 
	{
		UE_LOG(LogTemp, Warning, TEXT("We are taking damage for %f points"), _damageAmount);
		playerHealth -= _damageAmount;
		

		
		stunTime = _stunTime;
		
		if (characterState != ECharacterState::VE_Launched && stunTime > 0.0f)
		{
			characterState = ECharacterState::VE_Stunned;
			BeginStun();
		}
		
		PerformKnockback(_knockbackAmount, _launchAmount, true);

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

		stunTime = _blockstunTime;
	}

	if (characterState != ECharacterState::VE_Launched && stunTime > 0.0f)
	{
		characterState = ECharacterState::VE_Stunned;
		BeginStun();
	}
	else
	{
		characterState = ECharacterState::VE_Launched;
	}
	PerformKnockback(_knockbackAmount,  0.0f, false);
	if (playerHealth < 0.00f)
	{
		playerHealth = 0.00f;
	}
}

void AProjectAbyssV2Character::PerformKnockback(float _knockbackAmount, float _launchAmount, bool _hasBlocked)
{
	if (characterState ==  ECharacterState::VE_Blocking)
	{
		if (isFlipped)
		{
			LaunchCharacter(FVector(0.0f, _knockbackAmount * 2.0f, 0.0f), false, false);
		}
		else
		{
			LaunchCharacter(FVector(0.0f, -_knockbackAmount * 2.0f, 0.0f), false, false);
		}
	}
	else
	{
		if (_launchAmount > 0.0f)
		{
			UE_LOG(LogTemp, Warning, TEXT("We're being launched for %f."), _launchAmount);
			characterState = ECharacterState::VE_Launched;
			GetCharacterMovement()->GravityScale *= 0.7;
			if (isFlipped)
			{
				LaunchCharacter(FVector(0.0f, _knockbackAmount, _launchAmount), false, false);
			}
			else
			{
				LaunchCharacter(FVector(0.0f, -_knockbackAmount, _launchAmount), false, false);
			}
			
		}

	
	}
}

void AProjectAbyssV2Character::BeginStun()
{
	canMove = false;
	GetWorld()->GetTimerManager().SetTimer(stunTimerHandle, this, &AProjectAbyssV2Character::EndStun, stunTime, false);

}

void AProjectAbyssV2Character::EndStun()
{
	if (characterState != ECharacterState::VE_Launched)
	{
		characterState = ECharacterState::VE_Default;
	}
	
	canMove = true;
}

void AProjectAbyssV2Character::AddToInputMap(FString _input, EInputType _type)
{
	inputToInputTypeMap.Add(_input, _type);
}

void AProjectAbyssV2Character::AddtoBuffer(FInputInfo _inputInfo)
{
	inputBuffer.Add(_inputInfo);
	CheckBufferForCommandType();
}


//probably the most fragile function in this entire game. I've handled it in a way which should make it stable
//This checks the input buffer for commands, if it matches, the relevant attack is performed.
//Deprecated as a more efficient method is now used

void AProjectAbyssV2Character::CheckBufferForCommand()
{
	
}

void AProjectAbyssV2Character::CheckBufferForCommandType()
{
	int correctSequenceCounter = 0;

	for (auto currentCommand : characterCommands)
	{
		for (int commandInput = 0; commandInput < currentCommand.inputTypes.Num(); ++commandInput)
		{
			for (int input = 0; input < inputBuffer.Num(); ++input)
			{
				if (input + correctSequenceCounter < inputBuffer.Num())
				{
					if (inputBuffer[input + correctSequenceCounter].inputType == ((currentCommand.inputTypes[commandInput])))
					{
						UE_LOG(LogTemp, Warning, TEXT("The player added another input to the command sequance."));
						++correctSequenceCounter;

						if (correctSequenceCounter == currentCommand.inputTypes.Num())
						{
							StartCommand(currentCommand.name);
						}

						break;
					}
					else
					{
						UE_LOG(LogTemp, Warning, TEXT("The player broke the command sequence."));
						correctSequenceCounter = 0;
					}
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("The player is not yet finished with the command sequence."));
					correctSequenceCounter = 0;
				}
			}
		}
	}
}
	

void AProjectAbyssV2Character::StartCommand(FString _commandName)
{
	for (int currentCommand = 0; currentCommand < characterCommands.Num(); ++currentCommand)
	{
		if (_commandName.Compare(characterCommands[currentCommand].name) == 0)
		{
			characterCommands[currentCommand].hasUsedCommand = true;
		}
		if (_commandName.Compare(characterCommands[currentCommand].name) == 2)
		{
			characterCommands[currentCommand].hasUsedSuper = true;
		}

		if (characterCommands[currentCommand].hasUsedSuper)
		{
			StartTerrorAttack();
		}
	}
}

void AProjectAbyssV2Character::RemovefromBuffer()
{

}





//P2 actions 
//AGAIN PLEASE DO NOT LET PEOPLE TRY THIS, IT WON'T BE FUN
void AProjectAbyssV2Character::P2KeyboardJab()
{
	StartJab();
}

void AProjectAbyssV2Character::P2KeyboardStrong()
{
	StartStrong();
}

void AProjectAbyssV2Character::P2KeyboardFierce()
{
	StartFierce();
}

void AProjectAbyssV2Character::P2KeyboardShort()
{
	StartShort();
}

void AProjectAbyssV2Character::P2KeyboardLong()
{
	StartLong();
}

void AProjectAbyssV2Character::P2KeyboardRoundhouse()
{
	StartRoundhouse();
}

void AProjectAbyssV2Character::P2KeyboardJump()
{
	Jump();
}

void AProjectAbyssV2Character::P2KeyboardStopJumping()
{
	StopJumping();
}

void AProjectAbyssV2Character::P2KeyboardMoveRight(float _value)
{
	MoveRight(_value);
}


void AProjectAbyssV2Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
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
						//if (isFlipped)
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
			//				isFlipped = false;
		//				}
				//	}
			//		else
	//				{
					//	if (isFlipped)
		//					if (auto mesh = GetCapsuleComponent()->GetChildComponent(1))
						//	{
				//			//
					//			transform = mesh->GetRelativeTransform();
					//			scale = transform.GetScale3D();
				//				scale.Y = 1;
					//			transform.SetScale3D(scale);
				//				mesh->SetRelativeTransform(transform);

//							}
	//					isFlipped = true;
			//		}
				//}
			//}
		//}
//	}
//} 