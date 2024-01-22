// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ProjectAbyssV2Character.generated.h"

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

	void StartJab();
	void StartStrong();
	void StartFierce();
	void StartShort();
	void StartLong();
	void StartRoundhouse();

	//When in keyboard only mode use following functions to allow P2 to perform actions
	//Please do not allow people to put themselves through this
	//It will get messy
	UFUNCTION(BlueprintCallable)
		void P2KeyboardJab();

	UFUNCTION(BlueprintCallable)
		void P2KeyboardStrong();

	UFUNCTION(BlueprintCallable)
		void P2KeyboardFierce();

	UFUNCTION(BlueprintCallable)
		void P2KeyboardShort();

	UFUNCTION(BlueprintCallable)
		void P2KeyboardLong();

	UFUNCTION(BlueprintCallable)
		void P2KeyboardRoundhouse();

	UFUNCTION(BlueprintCallable)
		void P2KeyboardJump();

	UFUNCTION(BlueprintCallable)
		void P2KeyboardStopJumping();

	UFUNCTION(BlueprintCallable)
		void P2KeyboardMoveRight(float _value);

	//Please heed my warning

protected:
	void Tick(float DeltaTime);

	/** Called for side to side input */
	void MoveRight(float Val);

	/** Handle touch inputs. */
	void TouchStarted(const ETouchIndex::Type FingerIndex, const FVector Location);

	/** Handle touch stop event. */
	void TouchStopped(const ETouchIndex::Type FingerIndex, const FVector Location);

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	// End of APawn interface

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character)
	// ECharacterClass characterClass;

	//Who is it?
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		FString charName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hitbox")
		AActor* hurtbox;

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

	//damage the player
	UFUNCTION(BlueprintCallable)
	void TakeDamage(float _damageAmount);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player References")
		AProjectAbyssV2Character* otherPlayer;

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

	//Player Health Stat
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
		float playerHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
		float maxDistanceApart;

	//is the player's model flipped
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Model")
		bool isFlipped;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combos")
		bool atkHit;

public:
	AProjectAbyssV2Character();

	/** Returns SideViewCameraComponent subobject **/
	//FORCEINLINE class UCameraComponent* GetSideViewCameraComponent() const { return SideViewCameraComponent; }
	/** Returns CameraBoom subobject **/
	//FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
};
