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
		UE_LOG(LogTemp, Warning, TEXT("BOSS SPAWNED!"));
	}
}

void ATomAndJerryGameModeBase::DecrementNumMonsters()
{
	NumMonsters--;
	if (NumMonsters < 0)
		NumMonsters = 0;
}

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