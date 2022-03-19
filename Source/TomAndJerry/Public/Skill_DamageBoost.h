// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill.h"
#include "Skill_DamageBoost.generated.h"

/**
 * 
 */
UCLASS()
class TOMANDJERRY_API ASkill_DamageBoost : public ASkill
{
	GENERATED_BODY()
public:
	virtual bool CanExecute(const APawnMonster* Monster) const override;
	virtual void Execute() override;
	virtual uint8 GetEnergyCost() const override{return EnergyCost;}
	virtual void Destroyed() override;
protected:
	virtual void BeginPlay() override;
private:
	//The percent of the enemy Pawn's max health that its current health must at least be at for this skill to execute
	UPROPERTY(EditAnywhere, meta=(ClampMin = "0.0", ClampMax = "1.0"))
	float HealthPercentToExecute = 0.50;
	//How much to multiply the damage dealt by this Pawn to an enemy Pawn
	UPROPERTY(EditAnywhere, meta=(ClampMin = "1.0"))
	float DamageBonus = 1.50;
	//How long this Skill lasts
	UPROPERTY(EditAnywhere)
	float Duration = 15.0;
};
