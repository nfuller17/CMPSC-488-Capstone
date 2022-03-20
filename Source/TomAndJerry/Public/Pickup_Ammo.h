// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickup.h"
#include "Pickup_Ammo.generated.h"

/**
 * 
 */
UCLASS()
class TOMANDJERRY_API APickup_Ammo : public APickup
{
	GENERATED_BODY()
	
public:
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	
private:
	//How much health does this pickup provide
	UPROPERTY(EditAnywhere, meta=(ClampMin= "1"))
	uint16 AmmoAmount = 30;
};
