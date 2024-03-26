// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HitboxActor.generated.h"

UENUM(BlueprintType)
enum class EHitboxEnum : uint8
{
	HB_PROXIMITY	UMETA(DisplayName = "Proximity"),
	HB_STRIKE		UMETA(DisplayName = "Strike"),
	HB_HURTBOX		UMETA(DisplayName = "Hurtbox")
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


	//Damage the hitbox will do
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hitbox")
		float hitboxDamage;

	//Damage the hitbox will do
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hitbox")
		float hitstunTime;

	//Damage the hitbox will do
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hitbox")
		float blockStunTime;

	//Hitbox Enum instance
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hitbox")
		EHitboxEnum hitboxType;

	//Hitbox location
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hitbox")
		FVector hitboxLocation;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
