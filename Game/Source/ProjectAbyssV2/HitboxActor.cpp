// Fill out your copyright notice in the Description page of Project Settings.


#include "HitboxActor.h"

// Sets default values
AHitboxActor::AHitboxActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	hitboxDamage = 0.0f;
	hitstunTime = 0.0f;
	blockStunTime = 0.0f;
	knockbackAmount = 0.0f;
	launchDistance = 0.0f;
}

// Called when the game starts or when spawned
void AHitboxActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHitboxActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AHitboxActor::TriggerVisualizeHitbox()
{
	VisualizeHitbox();
}