// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "CustomSettings.generated.h"

/**
 * 
 */



UCLASS(Config = Game, defaultconfig, meta = (DisplayName = "config")) //Name for the class in UI
class PROJECTABYSSV2_API UCustomSettings : public UDeveloperSettings
{
	GENERATED_BODY()
	
};
