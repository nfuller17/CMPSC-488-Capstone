// Fill out your copyright notice in the Description page of Project Settings.


#include "ControllerJerry.h"
#include "WeaponMaterial.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

void AControllerJerry::BeginPlay()
{
	Super::BeginPlay();
	
	HUD = CreateWidget(this, HUDClass);
	if (HUD != nullptr)
	{
		HUD->AddToViewport();
	}
}

void AControllerJerry::Spectate(const bool& bSpectate)
{
	if (bSpectate)
	{
		UE_LOG(LogTemp, Warning, TEXT("Spectate"));
		BeginSpectatingState();
		ASpectatorPawn* SpecPawn = SpawnSpectatorPawn();
		if (SpecPawn == nullptr)
			return;
		if (HUD)
			HUD->RemoveFromViewport();
		APawn* JerryPawn = GetPawn();
		if (JerryPawn != nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Destroy old pawn!"));
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

void AControllerJerry::GameHasEnded(AActor* EndGameFocus, bool bIsWinner)
{
	Super::GameHasEnded(EndGameFocus, bIsWinner);
	if (HUD)
		HUD->RemoveFromViewport();
	GetWorldTimerManager().SetTimer(RestartTimer, this, &APlayerController::RestartLevel, RestartDelay);
}