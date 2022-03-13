// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PawnMonster.h"
#include "PawnMinion.generated.h"

//Base class for non-Boss AI enemies, extends PawnMonster

UCLASS()
class TOMANDJERRY_API APawnMinion : public APawnMonster
{
	GENERATED_BODY()

protected:
	virtual void Died() override;
};
