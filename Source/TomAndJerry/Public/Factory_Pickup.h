// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/NavigationObjectBase.h"
#include "Pickup.h"
#include "Factory_Pickup.generated.h"

class APickup;

UCLASS()
class TOMANDJERRY_API AFactory_Pickup : public ANavigationObjectBase
{
	GENERATED_BODY()
	
public:
	void SpawnPickup();
	void RestartTimer();
	FTimerHandle SpawnTimer;
	
protected:
	virtual void BeginPlay() override;
	
private:
	UPROPERTY(EditAnywhere)
	float RespawnRate = 30.0;
	UPROPERTY(EditAnywhere)
	TSubclassOf<APickup> PickupClass;
	
};
