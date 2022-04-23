// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickup_Ammo.h"
#include "PawnJerry.h"
#include "AmmoComponent.h"

void APickup_Ammo::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	if (OtherActor != nullptr)
	{
		APawnJerry* Player = Cast<APawnJerry>(OtherActor);
		if (!IsValid(Player))
			return;

		UAmmoComponent* AmmoComponent = Player->FindComponentByClass<UAmmoComponent>();
		if (!IsValid(AmmoComponent))
			return;
		if (AmmoComponent->GetAmmoAmount() >= AmmoComponent->GetAmmoMax())
			return;
		AmmoComponent->AddAmmo(AmmoAmount);
		Destroy();
	}
}