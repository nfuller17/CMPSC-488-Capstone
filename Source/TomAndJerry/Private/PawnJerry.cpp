// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnJerry.h"

// Sets default values
APawnJerry::APawnJerry()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APawnJerry::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APawnJerry::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APawnJerry::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

// Called to bind functionality to input
void APawnJerry::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &APawnJerry::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis(TEXT("Strafe"), this, &APawnJerry::Strafe);
	PlayerInputComponent->BindAxis(TEXT("LookRight"), this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis(TEXT("LookUpRate"), this, &APawnJerry::LookUpRate);
	PlayerInputComponent->BindAxis(TEXT("LookRightRate"), this, &APawnJerry::LookRightRate);
}

void APawnJerry::MoveForward(float AxisValue)
{
		AddMovementInput(GetActorForwardVector() * AxisValue);
}

void APawnJerry::Strafe(float AxisValue)
{
		AddMovementInput(GetActorRightVector() * AxisValue);
}

void APawnJerry::LookUpRate(float AxisValue)
{
	AddControllerPitchInput(AxisValue * RotationRate * GetWorld()->GetDeltaSeconds());
}

void APawnJerry::LookRightRate(float AxisValue)
{
	AddControllerYawInput(AxisValue * RotationRate * GetWorld()->GetDeltaSeconds());
}