// Fill out your copyright notice in the Description page of Project Settings.

//PawnJerry is the pawn class for human players

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PawnJerry.generated.h"

UCLASS()
class TOMANDJERRY_API APawnJerry : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APawnJerry();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
