// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnMonster.h"
#include "Components/SkeletalMeshComponent.h"

// Sets default values
APawnMonster::APawnMonster()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Health = HealthMax;
}

// Called when the game starts or when spawned
void APawnMonster::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APawnMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APawnMonster::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

