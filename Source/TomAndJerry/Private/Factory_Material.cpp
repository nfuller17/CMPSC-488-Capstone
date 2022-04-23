// Fill out your copyright notice in the Description page of Project Settings.


#include "Factory_Material.h"

void AFactory_Material::BeginPlay()
{
	Super::BeginPlay();
	SpawnMaterial();
}

void AFactory_Material::SpawnMaterial()
{
	AWeaponMaterial* MaterialInstance = GetWorld()->SpawnActor<AWeaponMaterial>(Material, GetActorLocation(), GetActorRotation());
	if (IsValid(MaterialInstance))
		MaterialInstance->SetFactory(this);
}