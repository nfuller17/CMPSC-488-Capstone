// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnJerrySpectator.h"
#include "EngineUtils.h"
#include "ControllerJerry.h"
#include "PawnMonster.h"
#include "../TomAndJerryGameModeBase.h"

void APawnJerrySpectator::BeginPlay()
{
	Super::BeginPlay();
}

void APawnJerrySpectator::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (IsValid(PlayerInputComponent))
	{
		PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &APawnJerrySpectator::MoveForward);
		PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &APawn::AddControllerPitchInput);
		PlayerInputComponent->BindAxis(TEXT("Strafe"), this, &APawnJerrySpectator::MoveRight);
		PlayerInputComponent->BindAxis(TEXT("LookRight"), this, &APawn::AddControllerYawInput);
		PlayerInputComponent->BindAction(TEXT("SpectateNextTarget"), EInputEvent::IE_Pressed, this, &APawnJerrySpectator::SpectateNextTarget);
		PlayerInputComponent->BindAction(TEXT("FreeSpectate"), EInputEvent::IE_Pressed, this, &APawnJerrySpectator::FreeSpectate);
		PlayerInputComponent->BindAction(TEXT("OpenMenu"), EInputEvent::IE_Pressed, this, &APawnJerrySpectator::OpenMenu);
	}
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

void APawnJerrySpectator::SpectateNextTarget()
{
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (!IsValid(PC))
		return;
	ATomAndJerryGameModeBase* Game = Cast<ATomAndJerryGameModeBase>(GetWorld()->GetAuthGameMode());
	if (!IsValid(Game))
		return;
	if (Game->GetSpectateMode())
	{
		int32 NumControllers = GetWorld()->GetNumControllers() - 1;
		int32 RandomIndex = FMath::RandRange(0, NumControllers);
		uint8 Counter = 0;
		for (auto Monster : TActorRange<APawnMonster>(GetWorld()))
		{
			if (Counter == RandomIndex)
			{
				PC->SetViewTarget(Monster);
				CurrentViewTarget = Monster;
				return;
			}
			Counter++;
		}
	}
}

void APawnJerrySpectator::OpenMenu()
{
	AControllerJerry* PC = Cast<AControllerJerry>(GetController());
	if (IsValid(PC))
		PC->OpenMenu();
}

void APawnJerrySpectator::FreeSpectate()
{
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (IsValid(PC))
		PC->SetViewTarget(this);

}