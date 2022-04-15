// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Factory_Pickup.h"
#include "Particles/ParticleSystemComponent.h"
#include "Pickup.generated.h"

class AFactory_Pickup;

UCLASS()
class TOMANDJERRY_API APickup : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickup();
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void SetPickupFactory(AFactory_Pickup* _PickupFactory){PickupFactory = _PickupFactory;}

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Destroyed() override;

private:	
	UPROPERTY(VisibleAnywhere)
		USceneComponent* Root;
	UPROPERTY(VisibleAnywhere)
		UParticleSystemComponent* Effect;
	AFactory_Pickup* PickupFactory;
	
};
