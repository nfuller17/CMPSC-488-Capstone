// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PawnMonster.h"
#include "PawnBoss.generated.h"

//Base class for Boss AI, extends PawnMonster
//If Boss dies (health = 0), game ends in win state for player

UCLASS()
class TOMANDJERRY_API APawnBoss : public APawnMonster
{
	GENERATED_BODY()

public:
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	virtual void Died() override;
};