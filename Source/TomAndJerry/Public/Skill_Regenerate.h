// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill.h"
#include "PawnMonster.h"
#include "Skill_Regenerate.generated.h"

/**
 * 
 */
UCLASS()
class TOMANDJERRY_API ASkill_Regenerate : public ASkill
{
	GENERATED_BODY()
public:
	virtual bool CanExecute(APawnMonster* Monster) override;
	virtual void Execute() override;
	void AddHealth();
	virtual void Destroyed() override;
	FTimerHandle RegenerationTimer;
protected:
	virtual void BeginPlay() override;
	uint8 EnergyCost=80;
private:
	float MinHealth=50.0;
	float RegenerationRate = 1.0;
	float RegenerationAmount = 5.0;
	float Duration = 20.0;
};
