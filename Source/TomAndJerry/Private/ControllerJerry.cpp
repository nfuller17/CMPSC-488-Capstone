// Fill out your copyright notice in the Description page of Project Settings.


#include "ControllerJerry.h"
#include "WeaponMaterial.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

void AControllerJerry::BeginPlay()
{
	Super::BeginPlay();
	if (WinScreen != nullptr)
		WinScreen->RemoveFromViewport();
	if (LoseScreen != nullptr)
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
		if (JerryPawn != nullptr)
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
	if (InGameMenu != nullptr)
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
	if (HUD != nullptr)
		HUD->AddToViewport();
}

void AControllerJerry::DestroyHUD()
{
	if (HUD != nullptr)
		HUD->RemoveFromViewport();
}

void AControllerJerry::GameHasEnded(AActor* EndGameFocus, bool bIsWinner)
{
	Super::GameHasEnded(EndGameFocus, bIsWinner);
	DestroyHUD();
	if (bIsWinner)
	{
		WinScreen = CreateWidget(this, WinScreenClass);
		if (WinScreen != nullptr)
			WinScreen->AddToViewport();
	}
	else
	{
		LoseScreen = CreateWidget(this, LoseScreenClass);
		if (LoseScreen != nullptr)
			LoseScreen->AddToViewport();
	}
	GetWorldTimerManager().SetTimer(RestartTimer, this, &APlayerController::RestartLevel, RestartDelay);
}