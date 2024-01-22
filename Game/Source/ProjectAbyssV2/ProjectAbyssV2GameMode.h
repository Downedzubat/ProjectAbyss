// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ProjectAbyssV2Character.h"
#include "ProjectAbyssV2GameMode.generated.h"



UCLASS(minimalapi)
class AProjectAbyssV2GameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AProjectAbyssV2GameMode();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player References")
	AProjectAbyssV2Character* player1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player References")
	AProjectAbyssV2Character* player2;

};



