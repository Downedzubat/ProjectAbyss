// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "HitboxActor.generated.h"

UENUM(BlueprintType)
enum class EHitboxEnum : uint8
{
	HB_PROXIMITY	UMETA(DisplayName = "Proximity"),
	HB_STRIKE		UMETA(DisplayName = "Strike"),
	HB_PROJECTILE		UMETA(DisplayName = "Projectile"),
	HB_THROW        UMETA(DisplayName = "Throw"),
	HB_HURTBOX		UMETA(DisplayName = "Hurtbox")
};

//Used to specify if a hitbox can essentially void certain guard types, for example, low beats standing block
UENUM(BlueprintType)
enum class EHitType : uint8
{
	E_LOW		UMETA(DisplayName = "Low"),
	E_MID		UMETA(DisplayName = "Mid"),
	E_HIGH		UMETA(DisplayName = "High"),
	E_OVERHEAD	UMETA(DisplayName = "Overhead"),
	E_NONE		UMETA(DisplayName = "None")
};

USTRUCT(BlueprintType)
struct FHitboxData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hitbox")
	float hitboxDamage;

	//Damage the hitbox will do
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hitbox")
	int hitstunFrames;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hitbox")
	float knockbackAmount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hitbox")
	float launchDistance;

	//Damage the hitbox will do
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hitbox")
	int blockStunFrames;

	//Hitbox Enum instance
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hitbox")
	EHitboxEnum hitboxType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hitbox")
	EHitType hitType;

	//Hitbox location
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hitbox")
	FTransform hitboxTransform;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hitbox")
	bool shouldAttach;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hitbox")
	bool shouldCauseGroundBounce;

};

UCLASS()
class PROJECTABYSSV2_API AHitboxActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHitboxActor();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Notify the HitboxActorBP class instance that the hitbox can be drawn
	UFUNCTION(BlueprintCallable)
		void TriggerVisualizeHitbox();

	//Draw hitbox to screen (Debug and practice only)
	UFUNCTION(BlueprintImplementableEvent)
		void VisualizeHitbox();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Hitbox")
	FHitboxData hitboxData;

	//Damage the hitbox will do
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hitbox")
		float hitboxDamage;

	//Damage the hitbox will do
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hitbox")
		int hitstunFrames;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hitbox")
		float knockbackAmount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hitbox")
		float launchDistance;

	//Damage the hitbox will do
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hitbox")
		int blockStunFrames;

	//Hitbox Enum instance
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hitbox")
		EHitboxEnum hitboxType;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hitbox")
		EHitType hitType;

	//Hitbox location
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hitbox")
		FVector hitboxLocation;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hitbox")
		bool shouldAttach;
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hitbox")
		bool shouldCauseGroundBounce;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
