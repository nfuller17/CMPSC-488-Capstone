// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MinorObjective.h"
#include "PawnAlly.h"
#include "ObjectiveAlly.generated.h"

/**
 * 
 */
UCLASS()
class TOMANDJERRY_API AObjectiveAlly : public AMinorObjective
{
	GENERATED_BODY()

public:
	// Called when player successfully completes Minor Objective. Set as virtual so child classes can override to provide their own rewards (weapons, allies, health, etc.)
	virtual void Complete() override;

private:
	UPROPERTY(EditAnywhere)
		TArray<TSubclassOf<APawnAlly>> Allies;
	//Max distance ally can spawn from this objective
	UPROPERTY(EditAnywhere)
		float SpawnRadius = 100.f;
	
};
