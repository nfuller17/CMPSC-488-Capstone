// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectiveWeapon.h"
#include "Weapon.h"
#include "PawnJerry.h"

void AObjectiveWeapon::Complete()
{
	Super::Complete();
	if (ActivatingPawn != nullptr)
	{
		APawnJerry* JerryPawn = Cast<APawnJerry>(ActivatingPawn);
		if (JerryPawn)
			JerryPawn->AddWeapon(WeaponClass);
	}
}

