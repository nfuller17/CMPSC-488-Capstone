// Fill out your copyright notice in the Description page of Project Settings.


#include "Factory_Pickup.h"

void AFactory_Pickup::BeginPlay()
{
	Super::BeginPlay();
	SpawnPickup();
}


//Only called when Pawn walks over pickup
void AFactory_Pickup::RestartTimer()
{
	GetWorldTimerManager().SetTimer(SpawnTimer, this, &AFactory_Pickup::SpawnPickup, RespawnRate, false, RespawnRate);
}

void AFactory_Pickup::SpawnPickup()
{
	if (Pickup != nullptr)	//Pickup for this factory already exists
		return;
	Pickup = GetWorld()->SpawnActor<APickup>(PickupClass, GetActorLocation(), GetActorForwardVector().Rotation());
	if (Pickup)
	{
		Pickup->SetPickupFactory(this);
	}
}