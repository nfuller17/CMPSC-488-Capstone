// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill.h"
#include "Skill_Amplify.generated.h"

/**
 * 
 */
UCLASS()
class TOMANDJERRY_API ASkill_Amplify : public ASkill
{
	GENERATED_BODY()
public:
	virtual void Execute() override;
	virtual uint8 GetEnergyCost() const override { return EnergyCost; }
	virtual void Destroyed() override;
	float GetFireRateMultiplier() const { return FireRateMultiplier; }

protected:
	virtual void BeginPlay() override;

private:
	//New fire rate will be multiplied by this value. Values < 1 = faster fire rate, > 1 = slower
	UPROPERTY(EditAnywhere)
	float FireRateMultiplier = 0.50;
	//How long this Skill lasts
	UPROPERTY(EditAnywhere)
		float Duration = 20.0;
};
