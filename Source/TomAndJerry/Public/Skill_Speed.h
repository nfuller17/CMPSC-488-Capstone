// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill.h"
#include "Skill_Speed.generated.h"

/**
 * 
 */
UCLASS()
class TOMANDJERRY_API ASkill_Speed : public ASkill
{
	GENERATED_BODY()
public:
	virtual void Execute() override;
	virtual uint8 GetEnergyCost() const override { return EnergyCost; }
	virtual void Destroyed() override;

protected:
	virtual void BeginPlay() override;

private:
	//How long this Skill lasts
	UPROPERTY(EditAnywhere)
		float Duration = 20.0;
	//How much to multiply the Pawn's walking speed
	UPROPERTY(EditAnywhere)
		float SpeedMultiplier = 2.0;
};
