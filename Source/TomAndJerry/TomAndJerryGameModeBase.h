// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Factory_Monster.h"
#include "Factory_Boss.h"
#include "Factory_Ally_SpectateMode.h"
#include "TomAndJerryGameModeBase.generated.h"

class AFactory_Monster;
class AFactory_Boss;
class AFactory_Ally_SpectateMode;

UCLASS()
class TOMANDJERRY_API ATomAndJerryGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	void AddMaterial(const uint8 Count);
	void EndGame(const bool bPlayerWon);
	FTimerHandle SpawnTimer;
	FTimerHandle AllySpawnTimer;
	uint8 GetNumMonsters(){return NumMonsters;}
	UFUNCTION(BlueprintPure)
		uint8 GetMaterialsCollected() { return MaterialsCollected; }
	UFUNCTION(BlueprintPure)
		uint8 GetTotalMaterials() { return MaterialsTotal;  }
	void DecrementNumMonsters();
	void DecrementNumAlliesForSpectate();
	void IncrementMCollected(const uint8 numMaterials);
	void DecrementNumLives() { NumLives--; }
	float GetPlayerSpawnDelay() const{return PlayerSpawnDelay;}
	UFUNCTION(BlueprintCallable)
	void SetSpectateMode(const bool& _SpectateMode) { SpectateMode = _SpectateMode; }
	bool GetSpectateMode() const { return SpectateMode; }
	UFUNCTION(BlueprintCallable)
	void SpawnAlliesIfSpectating(const bool& _SpectateMode);
	TArray<APawn*> SpectateList;
	
protected:
	virtual void BeginPlay() override;
	void SpawnMonster();
	void SpawnBoss();
	void SpawnAlly();
	
private:
	uint8	MaterialsCollected = 0;
	uint8	MaterialsTotal = 0;
	uint8   NumLives = 3;
	//Spawn delay for player
	UPROPERTY(EditAnywhere, meta=(ClampMin = "0.0"))
	float PlayerSpawnDelay = 3.0;
	//Time in seconds that monsters should spawn
	UPROPERTY(EditAnywhere, meta=(ClampMin = "0.0") )
		float	MonsterSpawnInterval = 10.0;
	//How many enemy monsters can be present in the game at once
	UPROPERTY(EditAnywhere, meta=(ClampMin = "0"))
		uint8	MaxMonsters = 50;
	uint8 NumMonsters = 0;
	//Timein seconds that allies should spawn for SPECTATE mode
	UPROPERTY(EditAnywhere, meta=(ClampMin = "0.0"))
	float AllySpawnInterval = 10.0;
	//How many allies can be present in the game at once for SPECTATE mode
	UPROPERTY(EditAnywhere, meta = (ClampMin = "0"))
		uint8	MaxAlliesForSpectate = 10;
	uint8 NumAlliesForSpectate;
	TArray<AFactory_Monster*> MonsterFactories;
	TArray<AFactory_Boss*> BossFactories;
	TArray<AFactory_Ally_SpectateMode*> AllyFactories;
	bool bBossSpawned = false;
	bool SpectateMode = false;
};
