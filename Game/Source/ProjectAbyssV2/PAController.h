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
	

	AProjectAbyssV2Character* possessedPawn;
};
