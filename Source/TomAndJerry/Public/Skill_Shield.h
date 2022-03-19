// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill.h"
#include "PawnMonster.h"
#include "Skill_Shield.generated.h"


UCLASS()
class TOMANDJERRY_API ASkill_Shield : public ASkill
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
	//The radius to search for hostile projectiles from this Pawn's location, upon which if found, will enable this Shield skill
	UPROPERTY(EditAnywhere)
	float SearchRadius=400.00;
	//The percentage of the damage that is taken off of a hit to this Pawn
	UPROPERTY(EditAnywhere, meta=(ClampMin = "0.0", ClampMax="1.0"))
	float DamageReduction = 0.50;
	//How long this Skill lasts
	UPROPERTY(EditAnywhere)
	float Duration = 20.0;
};
