// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TomAndJerryGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class TOMANDJERRY_API ATomAndJerryGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	void AddMaterial(const uint8 Count);
	void EndGame(const bool bPlayerWon);
	
protected:
	virtual void BeginPlay() override;
	
private:
	uint8	MaterialsCollected = 0;
	uint8	MaterialsTotal = 0;
};
