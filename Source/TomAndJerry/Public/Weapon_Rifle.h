// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "Weapon_Rifle.generated.h"

/**
 * 
 */
UCLASS()
class TOMANDJERRY_API AWeapon_Rifle : public AWeapon
{
	GENERATED_BODY()
	
public:
	virtual void FirePrimary() override;
};
