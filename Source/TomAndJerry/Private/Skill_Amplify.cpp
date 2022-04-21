// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill_Amplify.h"
#include "PawnJerry.h"
#include "Weapon.h"

void ASkill_Amplify::BeginPlay()
{
	Super::BeginPlay();
	SetLifeSpan(Duration);
}

void ASkill_Amplify::Execute()
{
	//Do not call Super!
	APawnJerry* Player = Cast<APawnJerry>(GetOwner());
	if (Player == nullptr)
		return;
	Player->SetAmplified(true);
	AWeapon* Weapon = Player->GetWeapon();
	if (Weapon == nullptr)
		return;
	Weapon->SetFireRate(Weapon->GetFireRate() * FireRateMultiplier);
	Weapon->SetAmmoPerFire(0);
}

void ASkill_Amplify::Destroyed()
{
	Super::Destroyed();
	APawnJerry* Player = Cast<APawnJerry>(GetOwner());
	if (Player != nullptr)
	{
		Player->SetAmplified(false);
		AWeapon* Weapon = Player->GetWeapon();
		if (Weapon != nullptr)
		{
			Weapon->SetFireRate(Weapon->GetFireRate() * 1 / FireRateMultiplier);
			//TODO: Restore AmmoPerFire to default instance
			Weapon->SetAmmoPerFire(1);
		}
	}
}