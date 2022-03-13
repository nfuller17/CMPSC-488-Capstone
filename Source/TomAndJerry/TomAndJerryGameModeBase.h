// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Factory_Monster.h"
#include "Factory_Boss.h"
#include "TomAndJerryGameModeBase.generated.h"

class AFactory_Monster;
class AFactory_Boss;

UCLASS()
class TOMANDJERRY_API ATomAndJerryGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	void AddMaterial(const uint8 Count);
	void EndGame(const bool bPlayerWon);
	FTimerHandle SpawnTimer;
	uint8 GetNumMonsters(){return NumMonsters;}
	void DecrementNumMonsters();
	
protected:
	virtual void BeginPlay() override;
	void SpawnMonster();
	void SpawnBoss();
	
private:
	uint8	MaterialsCollected = 0;
	uint8	MaterialsTotal = 0;
	//Time in seconds that monsters should spawn
	UPROPERTY(EditAnywhere, meta=(ClampMin = "0.0") )
		float	MonsterSpawnInterval = 10.0;
	//How many enemy monsters can be present in the game at once
	UPROPERTY(EditAnywhere, meta=(ClampMin = "0"))
		uint8	MaxMonsters = 10;
	uint8 NumMonsters = 0;
	TArray<AFactory_Monster*> MonsterFactories;
	TArray<AFactory_Boss*> BossFactories;
	bool bBossSpawned = false;
};
