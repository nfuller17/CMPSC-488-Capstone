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
	
	//Initialize Monster Factory array
	for (auto Factory: TActorRange<AFactory_Monster>(GetWorld()))
		MonsterFactories.Emplace(Factory);
	for (auto Factory: TActorRange<AFactory_Boss>(GetWorld()))
		BossFactories.Emplace(Factory);
	
	//Set timer to spawn monsters
	GetWorldTimerManager().SetTimer(SpawnTimer, this, &ATomAndJerryGameModeBase::SpawnMonster, MonsterSpawnInterval, true, 0);
}

void ATomAndJerryGameModeBase::SpawnAlliesIfSpectating(const bool& _SpectateMode)
{
	if (_SpectateMode)
	{
		UE_LOG(LogTemp, Warning, TEXT("Spectate mode set for game mode base!"));
		for (auto Factory : TActorRange<AFactory_Ally_SpectateMode>(GetWorld()))
			AllyFactories.Emplace(Factory);
		//Set timer to spawn monsters
		GetWorldTimerManager().SetTimer(AllySpawnTimer, this, &ATomAndJerryGameModeBase::SpawnAlly, AllySpawnInterval, true, 0);
	}
}

void ATomAndJerryGameModeBase::SpawnMonster()
{
	if (NumMonsters >= MaxMonsters)
		return;
	if (MonsterFactories.Num() == 0)
		return;
	AFactory_Monster* Factory = MonsterFactories[FMath::RandRange(0, MonsterFactories.Num()-1)];
	if (Factory == nullptr)
		return;
	if (Factory->SpawnMonster())
		NumMonsters++;
}

void ATomAndJerryGameModeBase::SpawnBoss()
{
	AFactory_Boss* Factory = BossFactories[FMath::RandRange(0, BossFactories.Num()-1)];
	if (Factory == nullptr)
		return;
	if (Factory->SpawnBoss())
	{
		bBossSpawned = true;
	}
}

//Only for spawning friendly AI in spectate mode
//For player spawning friendly AI, check out Factory_Ally
void ATomAndJerryGameModeBase::SpawnAlly()
{
	if (!SpectateMode) {	//Safety check
		GetWorldTimerManager().ClearTimer(AllySpawnTimer);
		return;
	}
	if (NumAlliesForSpectate >= MaxAlliesForSpectate)
		return;
	if (AllyFactories.Num() == 0)
		return;
	AFactory_Ally_SpectateMode* Factory = AllyFactories[FMath::RandRange(0, AllyFactories.Num() - 1)];
	if (Factory == nullptr)
		return;
	if (Factory->SpawnAlly())
		NumAlliesForSpectate++;
}

void ATomAndJerryGameModeBase::DecrementNumMonsters()
{
	NumMonsters--;
	if (NumMonsters < 0)
		NumMonsters = 0;
}

void ATomAndJerryGameModeBase::DecrementNumAlliesForSpectate()
{
	NumAlliesForSpectate--;
	if (NumAlliesForSpectate < 0)
		NumAlliesForSpectate = 0;
}

void ATomAndJerryGameModeBase::IncrementMCollected(const uint8 numMaterials) { MaterialsCollected += numMaterials; }

void ATomAndJerryGameModeBase::AddMaterial(const uint8 Count)
{
	//If player drops off a material, spawn a boss the first time they do so
	if (!bBossSpawned)
		SpawnBoss();
}

void ATomAndJerryGameModeBase::EndGame(const bool bPlayerWon)
{
	GetWorldTimerManager().ClearTimer(SpawnTimer);
	for (AController* Controller : TActorRange<AController>(GetWorld()))
	{
		bool bIsWinner = Controller->IsPlayerController() == bPlayerWon;
		Controller->GameHasEnded(Controller->GetPawn(), bIsWinner);
	}
}