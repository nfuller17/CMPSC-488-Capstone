// Fill out your copyright notice in the Description page of Project Settings.


#include "ControllerJerry.h"
#include "WeaponMaterial.h"

//Called when player walks over a material
//Adds the material to the MaterialInventory array
void AControllerJerry::AddWeaponMaterial(uint8 WeaponMaterialNumber)
{
	MaterialInventory.Emplace(WeaponMaterialNumber);
	for (uint8 x = 0; x < MaterialInventory.Num(); x++)
		UE_LOG(LogTemp, Warning, TEXT("Added Material %d to Inventory!"), WeaponMaterialNumber);
}

