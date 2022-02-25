// Copyright Epic Games, Inc. All Rights Reserved.


#include "TomAndJerryGameModeBase.h"
#include "EngineUtils.h"
#include "WeaponMaterial.h"

void ATomAndJerryGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	
	//Get a count of how many materials there are in the level
	for (auto Material: TActorRange<AWeaponMaterial>(GetWorld()))
		MaterialsTotal++;
	UE_LOG(LogTemp, Warning, TEXT("Game Started. Number of materials to collect: %d."), MaterialsTotal);
}

void ATomAndJerryGameModeBase::AddMaterial(const uint8 Count)
{
	MaterialsCollected += Count;
	if (MaterialsCollected >= MaterialsTotal)
		EndGame(true);
}

void ATomAndJerryGameModeBase::EndGame(const bool bPlayerWon)
{
	for (AController* Controller : TActorRange<AController>(GetWorld()))
	{
		bool bIsWinner = Controller->IsPlayerController() == bPlayerWon;
		Controller->GameHasEnded(Controller->GetPawn(), bIsWinner);
	}
}