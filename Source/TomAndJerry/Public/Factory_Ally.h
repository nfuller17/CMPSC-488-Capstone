// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/NavigationObjectBase.h"
#include "ObjectiveAlly.h"
#include "Factory_Ally.generated.h"

class AObjectiveAlly;

UCLASS()
class TOMANDJERRY_API AFactory_Ally : public ANavigationObjectBase
{
	GENERATED_BODY()
public:
	void RestartTimer();
	FTimerHandle SpawnTimer;
	void SpawnAllyObjective();
	void SetAllyObjective(AObjectiveAlly* _AllyObjective) { AllyObjective = _AllyObjective; }
protected:
	virtual void BeginPlay() override;
private:
	//Time in seconds this factory spawns another ally objective if previously completed
	UPROPERTY(EditAnywhere)
		float RespawnRate = 120.0;
	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<AObjectiveAlly>> AllyObjectives;
	AObjectiveAlly* AllyObjective;
};
