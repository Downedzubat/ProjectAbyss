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
	isFlipped = false;
	atkHit = false;

	playerHealth = 1.00f;
	maxDistanceApart = 600.0f;
	stunTime = 0.0f;
	canMove = true;

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
	//ACharacter::Landed(Hit);
	characterState = ECharacterState::VE_Default;
}

void AProjectAbyssV2Character::StartCrouching()
{
	isCrouching = true;
}

void AProjectAbyssV2Character::StopCrouching()
{
	isCrouching = false;
}

void AProjectAbyssV2Character::MoveRight(float Value)
{

	if (!isCrouching)
	{
		if (characterState != ECharacterState::VE_Jumping)
		{
			if (Value > 0.20f)
			{
				characterState = ECharacterState::VE_MovingRight;
			}
			else if (Value < -0.20f)
			{
				characterState = ECharacterState::VE_MovingLeft;
			}
			else
			{
				characterState = ECharacterState::VE_Default;
			}
		}
	}

	float currentDistanceApart = abs(otherPlayer->GetActorLocation().Y - GetActorLocation().Y);

	if (currentDistanceApart >= maxDistanceApart)
	{
		if ((currentDistanceApart + Value < currentDistanceApart && !isFlipped) || (currentDistanceApart - Value < currentDistanceApart && isFlipped))
		{
			// add movement in that direction
			AddMovementInput(FVector(0.f, -1.f, 0.f), Value);
		}
	}
	else
	{
		// add movement in that direction
		AddMovementInput(FVector(0.f, -1.f, 0.f), Value);
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
}

void AProjectAbyssV2Character::StartStrong()
{
	UE_LOG(LogTemp, Warning, TEXT("We are using STRONG"))
	wasStrongUsed = true;
}

void AProjectAbyssV2Character::StartFierce()
{
	UE_LOG(LogTemp, Warning, TEXT("We are using FIERCE"))
	wasFierceUsed = true;
}

void AProjectAbyssV2Character::StartShort()
{
	UE_LOG(LogTemp, Warning, TEXT("We are using SHORT"))
	wasShortUsed = true;
}

void AProjectAbyssV2Character::StartLong()
{
	UE_LOG(LogTemp, Warning, TEXT("We are using LONG"))
	wasLongUsed = true;
}

void AProjectAbyssV2Character::StartRoundhouse()
{
	UE_LOG(LogTemp, Warning, TEXT("We are using ROUNDHOUSE"))
	wasRoundhouseUsed = true;
}

void AProjectAbyssV2Character::TakeDamage(float _damageAmount, float _stunTime)
{
	UE_LOG(LogTemp, Warning, TEXT("We are taking damage for %f points"), _damageAmount);
	playerHealth -= _damageAmount;

	characterState = ECharacterState::VE_Stunned;
	stunTime = _stunTime;
	BeginStun();

	if (otherPlayer)
	{
		otherPlayer->atkHit = true;
	}

	if (playerHealth < 0.00f)
	{
		playerHealth = 0.00f;
	}
}

void AProjectAbyssV2Character::BeginStun()
{
	canMove = false;
	GetWorld()->GetTimerManager().SetTimer(stunTimerHandle, this, &AProjectAbyssV2Character::EndStun, stunTime, false);

}

void AProjectAbyssV2Character::EndStun()
{
	characterState = ECharacterState::VE_Default;
	canMove = true;
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
	if (characterState != ECharacterState::VE_Jumping) {


		if (otherPlayer)
		{
			if (auto characterMovement = GetCharacterMovement())
			{
				if (auto enemyMovement = otherPlayer->GetCharacterMovement())
				{
					if (enemyMovement->GetActorLocation().Y >= characterMovement->GetActorLocation().Y)
					{
						// the following code should 110% be using scale.Y, but for some reason with Mask, it has to be scale.Z I'll note this down and look into a fix -FIXED
						//Now when Mask is flipped she is disjointed from hurtbox, I think the best solution is going to be to make our own idle anim using cascadeur
						if (isFlipped)
						{
							if (auto mesh = GetCapsuleComponent()->GetChildComponent(1))
							{

								transform = mesh->GetRelativeTransform();
								scale = transform.GetScale3D();
								scale.Y = -1;
								transform.SetScale3D(scale);
								mesh->SetRelativeTransform(transform);

							}
							isFlipped = false;
						}
					}
					else
					{
						if (isFlipped)
							if (auto mesh = GetCapsuleComponent()->GetChildComponent(1))
							{

								transform = mesh->GetRelativeTransform();
								scale = transform.GetScale3D();
								scale.Y = 1;
								transform.SetScale3D(scale);
								mesh->SetRelativeTransform(transform);

							}
						isFlipped = true;
					}
				}
			}
		}
	}
}