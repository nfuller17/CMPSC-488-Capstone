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
	void SetPickup(APickup* _Pickup){Pickup = _Pickup;}
	
protected:
	virtual void BeginPlay() override;
	
private:
	UPROPERTY(EditAnywhere)
	float RespawnRate = 30.0;
	APickup* Pickup;
	UPROPERTY(EditAnywhere)
	TSubclassOf<APickup> PickupClass;
	
};
