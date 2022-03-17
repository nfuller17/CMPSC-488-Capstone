// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill.h"
#include "Skill_Regenerate.generated.h"

/**
 * 
 */
UCLASS()
class TOMANDJERRY_API ASkill_Regenerate : public ASkill
{
	GENERATED_BODY()
public:
	virtual bool Execute() override;
	void StopExecution();
	void AddHealth();
	FTimerHandle RegenerationTimer;
	FTimerHandle ExecutionTimer;
protected:
	uint8 EnergyCost=80;
private:
	float MinHealth=50.0;
	float RegenerationRate = 1.0;
	float RegenerationAmount = 5.0;
	float Duration = 30.0;
};
