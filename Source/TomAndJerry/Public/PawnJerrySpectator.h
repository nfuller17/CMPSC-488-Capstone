// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SpectatorPawn.h"
#include "PawnJerrySpectator.generated.h"

/**
 * 
 */
UCLASS()
class TOMANDJERRY_API APawnJerrySpectator : public ASpectatorPawn
{
	GENERATED_BODY()

public:
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void MoveForward(float Val) override;
	void MoveRight(float Val) override;
	
};
