// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "MainMenu.h"
#include "GameFramework/Character.h"
#include "HitboxActor.h"
#include "Containers/CircularBuffer.h"
#include "ProjectAbyssV2Character.generated.h"

UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	VE_Default     UMETA(DisplayName = "NOT_MOVING"),
	VE_MovingRight UMETA(DisplayName = "MOVING_RIGHT"),
	VE_MovingLeft  UMETA(DisplayName = "MOVING_LEFT"),
	VE_Jumping     UMETA(DisplayName = "JUMPING"),
	VE_Crouching   UMETA(DisplayName = "CROUCHING"),
	VE_Blocking    UMETA(DisplayName = "BLOCKING")
};

UENUM(BlueprintType)
enum class EComboState : uint8
{
	E_None	UMETA(DisplayName = "NO_COMBO_STATE"),
	E_MidStunned  UMETA(DisplayName = "MIDHIT"),
	E_LowStunned  UMETA(DisplayName = "LOWHIT"),
	E_HighStunned UMETA(DisplayName = "HIGHHIT"),
	E_Knockdown   UMETA(DisplayName = "KNOCKDOWN"),
	E_Recovery    UMETA(DisplayName = "RECOVERY"),
	E_FloorBounce UMETA(Displayname = "FLOOR_BOUNCE"),
	E_WallBounce  UMETA(DisplayName = "WALL_BOUNCE"),
	E_Launched    UMETA(DisplayName = "LAUNCHED")
};

UENUM(BlueprintType)
enum class EInputType : uint8
{
	E_None							UMETA(DisplayName = "NONE"),
	E_Forward						UMETA(DisplayName = "FORWARD"),
	E_Backward						UMETA(DisplayName = "BACKWARD"),
	E_Jump								UMETA(DisplayName = "JUMP"),
	E_Crouch							UMETA(DisplayName = "CROUCH"),
	E_Block								UMETA(DisplayName = "BLOCK"),
	E_Jab								UMETA(DisplayName = "JAB"),
	E_Strong							UMETA(DisplayName = "STRONG"),
	E_Fierce							UMETA(DisplayName = "FIERCE"),
	E_Short								UMETA(DisplayName = "SHORT"),
	E_Long								UMETA(DisplayName = "LONG"),
	E_Roundhouse					UMETA(DisplayName = "ROUNDHOUSE"),
	//E_SpecialAttack					UMETA(DisplayName = "SPECIAL_ATTACK")
};

UENUM(BlueprintType)
enum class EInputStatus : uint8
{
	E_None		UMETA(DisplayName = "NONE"),
	E_Press		UMETA(DisplayName = "PRESS"),
	E_Release	UMETA(DisplayName = "RELEASE"),
	E_Hold		UMETA(DisplayName = "HOLD")
};

UENUM(BlueprintType)
enum class EAttackState : uint8
{
	E_None				UMETA(DisplayName = "NONE"),
	E_AttackStarted		UMETA(DisplayName = "ATTACK_STARTUP"),
	E_AttackActive		UMETA(DisplayName = "ATTACK_ACTIVE"),
	E_AttackRecovery	UMETA(DisplayName = "ATTACK_RECOVERY")
};

//This struct is used to form commands - chaining them together gives us special moves
USTRUCT(BlueprintType)
struct FCommandInput
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	EInputType inputType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	EInputStatus inputStatus;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	int64 requiredChargeFrames;
};


//The following parameters are what info can be edited about a specific command
USTRUCT(BlueprintType)
struct FCommand
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
		FString name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
		TArray<FCommandInput> inputTypes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	int64 maxFramesBetweenInputs = 12;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
		bool hasUsedCommand;
	
		//this is not in use, but removing it might cause issues currently
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
		bool hasUsedSuper;
};




//Raw input information
USTRUCT(BlueprintType)
struct FInputInfo
{
		GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	EInputType inputType;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	EInputStatus inputStatus;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	int64 frame;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	int64 chargedFrames;

};

USTRUCT(BlueprintType)
struct FChargeInputs
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	EInputType inputType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	int64 chargeFrames;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	bool isHoldingInput = false;
};

//max inputs to be captured per frame
const int inputsPerFrame = 8;

struct FInputInfoArray
{
public:
	FInputInfo inputs[inputsPerFrame];
};

USTRUCT(BlueprintType)
struct FDefaultHurtboxDetails
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hurtboxes")
	FVector scale;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hurtboxes")
	FVector offset;
};

UCLASS(config=Game)
class AProjectAbyssV2Character : public ACharacter
{
	GENERATED_BODY()

	/** Side view camera */
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	//class UCameraComponent* SideViewCameraComponent;

	/** Camera boom positioning the camera beside the character */
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	//class USpringArmComponent* CameraBoom;




protected:
	void Tick(float DeltaTime);


	

	

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	// ECharacterClass characterClass;
public: 
	/** Called for side to side input */
	void MoveRight(float Val);
	virtual void Jump() override;
	void MoveRightController(float Val);
	/** Handle touch inputs. */
	void TouchStarted(const ETouchIndex::Type FingerIndex, const FVector Location);

	/** Handle touch stop event. */
	void TouchStopped(const ETouchIndex::Type FingerIndex, const FVector Location);

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	// End of APawn interface
	//Character Stat Vars
	//Who is it?
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	FString charName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	FString winQuote;

	//Player Health Stat
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float playerHealth;

	// MOVEMENT VARS
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		bool canMove;

	//The current state of the character
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Details")
	ECharacterState characterState;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Details")
	EComboState comboState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Details")
	EAttackState attackState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float jumpHeight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float jumpDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	int jumpCount;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	int maxJumpCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	bool canFlip;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float gravityScale;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	bool isCrouching;

	//The amount of frames which an attack will stun for
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	int stunFrames;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float forwardDashDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float backDashDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float hitstopModifier;


	//ATTACK VARS
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attacks")
	bool canAttack;

	//Player Normals
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Normals")
	bool wasJabUsed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Normals")
	bool wasStrongUsed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Normals")
	bool wasFierceUsed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Normals")
	bool wasShortUsed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Normals")
	bool wasLongUsed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Normals")
	bool wasRoundhouseUsed;

	//HITBOX VARS

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hitbox")
		TArray<UStaticMeshComponent*> hurtboxArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hurtboxes")
	TArray<FDefaultHurtboxDetails> hurtboxDefaults;

	//INPUT VARS

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TArray<FCommand> characterCommands;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TMap<FString, EInputType> inputToInputTypeMap;

	UFUNCTION(BlueprintImplementableEvent)
	void PerformInputLogic(EInputType _type, EInputStatus _status);

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TCircularBuffer<FInputInfoArray> inputBuffer = TCircularBuffer<FInputInfoArray>(60);


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TArray<FCommand> moveBuffer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterDetails")
	ECharacterClass characterClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Model")
	FTransform startPos;
	//the character's transform
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Model")
		FTransform transform;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Model")
		FVector locationT;

	//the character's scale
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Model")
		FVector scale;
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Model")
		FVector location;

	//Override ACharacter and APawn functionality to have more control over jumping and landing
	//AKA..............
	//BEGONE AIR MOVEMENT CONTROL

	virtual void StopJumping() override;
	virtual void Landed(const FHitResult& Hit) override;

	float removeInputFromBufferTime;

	bool isPressingBackward;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player References")
		AProjectAbyssV2Character* otherPlayer;

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	TArray<USceneComponent*> capsuleChildren;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	USceneComponent* characterMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movelists")
		bool wasTerrorAtkUsed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
		float maxDistanceApart;
	//is the player's model flipped
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Model")
		bool isFacingRight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combos")
		bool atkHit;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input Stack")
		bool hasReleasedRightAxisInput;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input Stack")
		bool hasReleasedLeftAxisInput;
	// SuperMeter (called terror for upcoming game)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Meter")
		float terrorGauge;
	//Deprecated
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Commands")
		bool hasUsedTempCommand;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	bool isReadyForEntrance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	bool hasLostRound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	bool hasWonMatch;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameLogic")
	int roundsWon;
	int curTick;
	bool capturedInputThisFrame;
	int currentInputsThisFrame;

	//values for charge moves
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TArray<FChargeInputs> chargeTimes;

	// FUNCTIONS
	
	UFUNCTION(BlueprintCallable)
	void CustomLaunchCharacter(FVector _launchVelocity, bool _shouldOverrideXY, bool _shouldOverrideZ, bool _shouldIgnoreCharacterCollision = false);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void IgnorePlayerToPlayerCollision(bool _shouldIgnore);


	UFUNCTION(BlueprintImplementableEvent)
	void MoveCharacterSmoothly(FVector _start, FVector _end);

	void BeginHitstop(float _damageAmount);

	



	UFUNCTION(BlueprintCallable)
	void CollidedWithProximityHitbox();

	UFUNCTION(BlueprintCallable)
	void WinRound();

	UFUNCTION(BlueprintCallable)
	void WinMatch();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void NotifyRoundStart();

	UFUNCTION(BlueprintImplementableEvent)
	void NotifyRoundEnd();


	UFUNCTION(BlueprintCallable)
	void KO();


	UFUNCTION(BlueprintCallable)
	void DoubleKO();

	UFUNCTION(BlueprintImplementableEvent)
	void NotifyKO();

	UFUNCTION(BlueprintImplementableEvent)
	void NotifyDoubleKO();

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateHUDRoundIcons();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void NotifyPlayerLockedIn();

	//Timer for handling stuns
	FTimerHandle stunTimerHandle;

	//Stun state start 
	//(yes this is starting to get good)
	void BeginStun();

	//Stun State End
	//Can you feel it too? the whole game starting to come together
	void EndStun();

	UFUNCTION(BlueprintCallable)
	void RemovefromBuffer();

	//damage the player
	UFUNCTION(BlueprintCallable)
	void TakeDamage(float _damageAmount, int _hitstunFrames, int _blockstunFrames, float _launchAmount, float _knockbackAmount, EHitType _HitType);

	void PerformKnockback(float _knockbackAmount, float _launchAmount, bool _hasBlocked);

	UFUNCTION(BlueprintCallable)
	void AddToInputMap(FString _input, EInputType _type);

	UFUNCTION(BlueprintCallable)
	void AddtoBuffer(FInputInfo _inputInfo);

	//Checks buffer for sequence
	UFUNCTION(BlueprintCallable)
	void CheckBufferForCommandType(EInputStatus _inputStatus);

	bool MultiInputCommand(FCommand _command, EInputType _pressedInput);
	UFUNCTION(BlueprintCallable)
	void ChargeTimeTrackStart(FChargeInputs _inputToTrack);

	UFUNCTION(BlueprintCallable)
	void ChargeTimeTrackReset(FChargeInputs _inputToReset);

	//Determine which command should be executed based on specific criteria
	UFUNCTION(BlueprintCallable)
	void DetermineCommandToUse();

	//Performs the command if it matches one on the character
	UFUNCTION(BlueprintCallable)
	void StartCommand(FString _commandName);


	UFUNCTION(BlueprintCallable)
	void StartProxBlock();

	UFUNCTION(BlueprintCallable)
	void StopProxBlock();

	//----------------------------------------------------------------------------------------------------------------------------------------------------------
public:
	AProjectAbyssV2Character();
	void EndHitstop();
	/** Returns SideViewCameraComponent subobject **/
	//FORCEINLINE class UCameraComponent* GetSideViewCameraComponent() const { return SideViewCameraComponent; }
	/** Returns CameraBoom subobject **/
	//FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

//public functions
	public:
		//Make the player begin crouching
		UFUNCTION(BlueprintCallable)
		void StartCrouching();
		//Make the player stop crouching
		UFUNCTION(BlueprintCallable)
		void StopCrouching();
		void StartJab();
		void ReleaseJab();
		void StartStrong();
		void ReleaseStrong();
		void StartFierce();
		void ReleaseFierce();
		void StartShort();
		void ReleaseShort();
		void StartLong();
		void ReleaseLong();
		void StartRoundhouse();
		void ReleaseRoundhouse();
		void StartTerrorAttack();
};
