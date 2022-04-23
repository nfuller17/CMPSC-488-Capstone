// Fill out your copyright notice in the Description page of Project Settings.


#include "ControllerJerry.h"
#include "WeaponMaterial.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

void AControllerJerry::BeginPlay()
{
	Super::BeginPlay();
	if (IsValid(WinScreen))
		WinScreen->RemoveFromViewport();
	if (IsValid(LoseScreen))
		LoseScreen->RemoveFromViewport();
}

void AControllerJerry::Spectate(const bool& bSpectate)
{
	if (bSpectate)
	{
		BeginSpectatingState();
		ASpectatorPawn* SpecPawn = GetWorld()->SpawnActor<ASpectatorPawn>(JerrySpectatorPawn, GetSpawnLocation(), GetControlRotation());
		if (SpecPawn == nullptr)
			return;
		APawn* JerryPawn = GetPawn();
		if (IsValid(JerryPawn))
		{
			JerryPawn->Destroy();
		}
		Possess(SpecPawn);
	}
}

ASpectatorPawn* AControllerJerry::SpawnSpectatorPawn()
{
	Super::SpawnSpectatorPawn();
	return GetWorld()->SpawnActor<ASpectatorPawn>(JerrySpectatorPawn);
}

void AControllerJerry::OpenMenu()
{
	InGameMenu = CreateWidget(this, InGameMenuClass);
	if (IsValid(InGameMenu))
	{
		UGameplayStatics::SetGamePaused(GetWorld(), true);
		InGameMenu->AddToViewport();
		bShowMouseCursor = true;
		bEnableClickEvents = true;
		bEnableMouseOverEvents = true;
	}
}

void AControllerJerry::CreateHUD()
{
	if (HUD != nullptr)	//HUD already exists
		return;
	HUD = CreateWidget(this, HUDClass);
	if (IsValid(HUD))
		HUD->AddToViewport();
}

void AControllerJerry::DestroyHUD()
{
	if (IsValid(HUD))
		HUD->RemoveFromViewport();
}

void AControllerJerry::GameHasEnded(AActor* EndGameFocus, bool bIsWinner)
{
	Super::GameHasEnded(EndGameFocus, bIsWinner);
	DestroyHUD();
	if (bIsWinner)
	{
		WinScreen = CreateWidget(this, WinScreenClass);
		if (IsValid(WinScreen))
			WinScreen->AddToViewport();
	}
	else
	{
		LoseScreen = CreateWidget(this, LoseScreenClass);
		if (IsValid(LoseScreen))
			LoseScreen->AddToViewport();
	}
	GetWorldTimerManager().SetTimer(RestartTimer, this, &APlayerController::RestartLevel, RestartDelay);
}