// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon_Rifle.h"

void AWeapon_Rifle::FirePrimary()
{
	Super::FirePrimary();
	UE_LOG(LogTemp, Warning, TEXT("Firing Rifle"));
}