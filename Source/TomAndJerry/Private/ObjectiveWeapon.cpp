// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectiveWeapon.h"
#include "Weapon.h"
#include "PawnJerry.h"
#include "Kismet/GameplayStatics.h"

void AObjectiveWeapon::Complete()
{
	Super::Complete();
	if (ActivatingPawn != nullptr)
	{
		APawnJerry* JerryPawn = Cast<APawnJerry>(ActivatingPawn);
		if (IsValid(JerryPawn))
		{
			JerryPawn->AddWeapon(WeaponClass);
			if (IsValid(CollectEffect))
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), CollectEffect, GetActorLocation(), GetActorRotation());
		}
	}
}

