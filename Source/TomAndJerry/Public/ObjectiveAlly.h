// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MinorObjective.h"
#include "PawnAlly.h"
#include "Factory_Ally.h"
#include "ObjectiveAlly.generated.h"

class AFactory_Ally;

UCLASS()
class TOMANDJERRY_API AObjectiveAlly : public AMinorObjective
{
	GENERATED_BODY()

public:
	// Called when player successfully completes Minor Objective. Set as virtual so child classes can override to provide their own rewards (weapons, allies, health, etc.)
	virtual void Complete() override;
	void SetFactory(AFactory_Ally* _Factory) { Factory = _Factory; }
	virtual void Destroyed() override;

private:
	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<APawnAlly>> Allies;
	AFactory_Ally* Factory;
};
