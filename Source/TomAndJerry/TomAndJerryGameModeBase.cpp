// Copyright Epic Games, Inc. All Rights Reserved.


#include "TomAndJerryGameModeBase.h"
#include "EngineUtils.h"
#include "Factory_Material.h"
#include "Kismet/GameplayStatics.h"

void ATomAndJerryGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	
	//Get a count of how many materials there are in the level
	for (auto Material: TActorRange<AFactory_Material>(GetWorld()))
		MaterialsTotal++;
	UE_LOG(LogTemp, Warning, TEXT("Game Started. Number of materials to collect: %d."), MaterialsTotal);
	
	//Initialize Monster Factory array
	for (auto Factory: TActorRange<AFactory_Monster>(GetWorld()))
		MonsterFactories.Emplace(Factory);
	for (auto Factory: TActorRange<AFactory_Boss>(GetWorld()))
		BossFactories.Emplace(Factory);

	//Reserve space for SpectateList, which can be at most the number of minions and allies spawned in the game at once
	//SpectateList.Reserve(MaxMonsters + MaxAlliesForSpectate);
	
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
	if (BossFactories.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("CAUTION: BOSS FACTORY ARRAY IN TOMANDJERRYGAMEMODEBASE IS EMPTY"));
		return;
	}
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

void ATomAndJerryGameModeBase::Restart(APawnJerry* Jerry)
{
	// AGameMode* Game = Cast<AGameMode>this;
	// Game->RestartGame(); 
	APlayerController* JController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	JController->ConsoleCommand("RestartLevel");
}

void ATomAndJerryGameModeBase::DepositMaterial(const uint8& Count, APawnJerry* Player)
{
	if (Count <= 0)
		return;

	MaterialsCollected += Count;

	//If player drops off a material, spawn a boss the first time they do so
	if (!bBossSpawned)
		SpawnBoss();
	// ----------- For Test ---------------------
	// ----------- Remove after finished --------
	if (Player != nullptr && !ReceivedSuperWeapon)
	{
		SpawnSuperWeapon(Player);
		ReceivedSuperWeapon = true;
	}

	/*
	//Spawn a super weapon when all materials have been collected
	if (MaterialsCollected >= MaterialsTotal && Player != nullptr && !ReceivedSuperWeapon)
	{
		SpawnSuperWeapon(Player);
		//If pawn dies, this boolean can be used so they respawn with the super weapon instead of having to  re-collect materials again
		ReceivedSuperWeapon = true;
	}
	*/
}

//Called either DepositMaterial the first time player deposits material,
//Or when player respawns after dying with a super holding
void ATomAndJerryGameModeBase::SpawnSuperWeapon(APawnJerry* Player)
{
	if (SuperWeapons.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("CAUTION - SUPER WEAPONS ARRAY IN TOMANDJERRYGAMEMODEBASE IS EMPTY!"));
		return;
	}
	if (Player == nullptr)
		return;
	//Select a random super weapon
	TSubclassOf<AWeapon> SuperWeaponClass = SuperWeapons[FMath::RandRange(0, SuperWeapons.Num() - 1)];

	//Add super weapon to player inventory
	Player->AddWeapon(SuperWeaponClass);
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