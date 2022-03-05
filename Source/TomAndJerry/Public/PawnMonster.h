// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PawnMonster.generated.h"

//Base class for AI enemies

UCLASS(abstract)
class TOMANDJERRY_API APawnMonster : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APawnMonster();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void SetHealth(uint32 NewHealth) { Health = NewHealth; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(VisibleAnywhere)
		float Health;

private:
	UPROPERTY(EditAnywhere)
		float HealthMax = 100;
	UPROPERTY(EditAnywhere, Category="Combat")
		bool bIsMeleeOnly;

};
