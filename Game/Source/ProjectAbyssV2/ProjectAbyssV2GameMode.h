// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ProjectAbyssV2Character.h"
#include "ProjectAbyssV2GameMode.generated.h"


UENUM(BlueprintType)
enum class EMatchState : uint8
{
	E_Default	UMETA(DisplayName = "NO_STATE"),
	E_Character1Intro UMETA(DisplayName = "CHARACTER1_INTRO"),
	E_Character2Intro UMETA(DisplayName = "CHARACTER2_INTRO"),
	E_MatchBegin	UMETA(DisplayName = "MATCH_BEGIN"),
	E_MatchActive	UMETA(DisplayName = "MATCH_ACTIVE"),
	E_RoundOver		UMETA(DisplayName = "ROUND_OVER"),
	E_MatchOver		UMETA(DisplayName = "MATCH_OVER"),
	E_CharacterLoss	UMETA(DisplayName = "CHARACTER_LOSS"),
	E_CharacterWin	UMETA(DisplayName = "CHARACTER_WIN"),
	E_MatchResults	UMETA(DisplayName = "MATCH_RESULTS")
};


UCLASS(minimalapi)
class AProjectAbyssV2GameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AProjectAbyssV2GameMode();

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintImplementableEvent)
	void BPTick(float DeltaTime);


	UFUNCTION(BlueprintCallable)
	void RoundWin(AProjectAbyssV2Character* _winningCharacter = nullptr);

	UFUNCTION(BlueprintCallable)
	void MatchWon(AProjectAbyssV2Character* _winningCharacter = nullptr);
	
	
	UFUNCTION(BlueprintImplementableEvent)
	void TriggerMatchWinEffects(AProjectAbyssV2Character* _winningCharacter = nullptr);


	UFUNCTION(BlueprintImplementableEvent)
	void PauseGame();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player References")
	AProjectAbyssV2Character* player1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player References")
	AProjectAbyssV2Character* player2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Match State")
	EMatchState matchState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gamemode Settings")
	float roundTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gamemode Settings")
	int numRounds;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gamemode Settings")
	bool isTimerActive;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	int hitstopFrames;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameMode Settings")
	bool shouldSkipAllCharacterEntrances;

};



