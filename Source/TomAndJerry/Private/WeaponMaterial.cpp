// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponMaterial.h"

// Sets default values
AWeaponMaterial::AWeaponMaterial()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AWeaponMaterial::SetMaterialID(uint8 mID) { MaterialID = mID; }


// Called when the game starts or when spawned
void AWeaponMaterial::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AWeaponMaterial::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

