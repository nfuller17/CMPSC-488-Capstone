// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MinorObjective.h"
#include "GameFramework/Actor.h"
#include "ObjectiveWeapon.generated.h"

/**
 * 
 */
 
//Forward declare the Weapon class
class AWeapon;
 
UCLASS()
class TOMANDJERRY_API AObjectiveWeapon : public AMinorObjective
{
	GENERATED_BODY()
	
public:
	// Called when player successfully completes Minor Objective. Set as virtual so child classes can override to provide their own rewards (weapons, allies, health, etc.)
	virtual void Complete() override;
	
private:
	//The weapon class to provide the player
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<AWeapon> WeaponClass;
	UPROPERTY(EditAnywhere)
		UParticleSystem* CollectEffect;
};
