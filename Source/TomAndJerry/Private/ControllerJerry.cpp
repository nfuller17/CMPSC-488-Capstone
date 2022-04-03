// Fill out your copyright notice in the Description page of Project Settings.


#include "ControllerJerry.h"
#include "WeaponMaterial.h"
#include "Blueprint/UserWidget.h"

void AControllerJerry::BeginPlay()
{
	Super::BeginPlay();
	
	HUD = CreateWidget(this, HUDClass);
	if (HUD != nullptr)
	{
		HUD->AddToViewport();
	}
}

//Called when player walks over a material
//Adds the material to the MaterialInventory array
void AControllerJerry::AddWeaponMaterial(uint8 WeaponMaterialNumber)
{
	MaterialInventory.Emplace(WeaponMaterialNumber);
	UE_LOG(LogTemp, Warning, TEXT("Added Material %d to Inventory!"), WeaponMaterialNumber);
}

//Called by CraftingPost when player walks into crafting location
void AControllerJerry::CollectMaterials()
{
	MaterialInventory.Empty();
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

void AControllerJerry::GameHasEnded(AActor* EndGameFocus, bool bIsWinner)
{
	Super::GameHasEnded(EndGameFocus, bIsWinner);
	if (HUD)
		HUD->RemoveFromViewport();
	GetWorldTimerManager().SetTimer(RestartTimer, this, &APlayerController::RestartLevel, RestartDelay);
}