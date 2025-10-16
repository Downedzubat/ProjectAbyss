// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ProjectAbyssV2Character.h"
#include "ProjectAbyssV2GameMode.h"
#include "PAController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTABYSSV2_API APAController : public APlayerController
{
	GENERATED_BODY()

public:
	APAController();

	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void CallMoveRight(float _value);

	UFUNCTION(BlueprintCallable)
	void CallJump();

	UFUNCTION(BlueprintCallable)
	void CallStopJumping();

	UFUNCTION(BlueprintCallable)
	void CallStartCrouching();

	UFUNCTION(BlueprintCallable)
	void CallStopCrouching();

	UFUNCTION(BlueprintCallable)
	void CallStartJab();

	UFUNCTION(BlueprintCallable)
	void CallStartStrong();

	UFUNCTION(BlueprintCallable)
	void CallStartFierce();
	UFUNCTION(BlueprintCallable)
	void CallStartShort();
	UFUNCTION(BlueprintCallable)
	void CallStartLong();
	UFUNCTION(BlueprintCallable)
	void CallStartRoundhouse();
	UFUNCTION(BlueprintCallable)
	void PerformStartInputLogic();
	UFUNCTION(BlueprintCallable)
	void CallStartTerrorAttack();
	

	AProjectAbyssV2Character* possessedPawn;
};
