// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectiveWeapon.h"
#include "Weapon.h"

void AObjectiveWeapon::Complete()
{
	Super::Complete();
	//Spawn a weapon for the player
	GetWorld()->SpawnActor<AWeapon>(WeaponClass);
}

