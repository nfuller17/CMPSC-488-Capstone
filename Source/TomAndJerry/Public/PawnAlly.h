// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PawnMonster.h"
#include "PawnAlly.generated.h"

/**
 * 
 */
UCLASS()
class TOMANDJERRY_API APawnAlly : public APawnMonster
{
	GENERATED_BODY()
protected:
	virtual void Died() override;
};
