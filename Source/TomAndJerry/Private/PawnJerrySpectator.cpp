// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnJerrySpectator.h"

void APawnJerrySpectator::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &APawnJerrySpectator::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis(TEXT("Strafe"), this, &APawnJerrySpectator::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("LookRight"), this, &APawn::AddControllerYawInput);
}

void APawnJerrySpectator::MoveForward(float Val)
{
	Super::MoveForward(Val);
	AddMovementInput(GetActorForwardVector() * Val);
}

void APawnJerrySpectator::MoveRight(float Val)
{
	Super::MoveRight(Val);
	AddMovementInput(GetActorRightVector() * Val);
}