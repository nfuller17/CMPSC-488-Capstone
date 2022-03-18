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
	virtual bool CanExecute(const APawnMonster* Monster) const override;
	virtual void Execute() override;
	virtual uint8 GetEnergyCost() const override{return EnergyCost;}
	void AddHealth();
	FTimerHandle RegenerationTimer;
protected:
	virtual void BeginPlay() override;
private:
	//The health below which this skill can be activated to regenerate the owner's health
	UPROPERTY(EditAnywhere)
	float MinHealth=50.0;
	//The interval in seconds to regenerate health
	UPROPERTY(EditAnywhere)
	float RegenerationRate = 1.0;
	//The amount of healing per interval
	UPROPERTY(EditAnywhere)
	float RegenerationAmount = 5.0;
	//How long this Skill lasts
	UPROPERTY(EditAnywhere)
	float Duration = 20.0;
};
