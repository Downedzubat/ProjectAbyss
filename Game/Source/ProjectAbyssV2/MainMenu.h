// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MainMenu.generated.h"


UENUM(BlueprintType)
enum class ECharacterClass : uint8
{
	VE_Default		  UMETA(DisplayName = "Mannequin"),
	VE_MASK		  UMETA(DisplayName = "Mask"),
	VE_FIGHTMAN UMETA(DisplayName = "FightMan")
};
/**
 * 
 */
UCLASS()
class PROJECTABYSSV2_API UMainMenu : public UGameInstance
{
	GENERATED_BODY()
	
	public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player References")
		ECharacterClass characterClass;
};
