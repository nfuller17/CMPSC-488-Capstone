// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickup.h"
#include "Pickup_Health.generated.h"

/**
 * 
 */
UCLASS()
class TOMANDJERRY_API APickup_Health : public APickup
{
	GENERATED_BODY()

public:
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	
private:
	//How much health does this pickup provide
	UPROPERTY(EditAnywhere)
	float HealthAmount = 20.0;
};
