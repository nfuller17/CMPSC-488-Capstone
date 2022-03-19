// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill_Shield.h"
#include "EngineUtils.h"
#include "Projectile.h"

void ASkill_Shield::BeginPlay()
{
	Super::BeginPlay();
	SetLifeSpan(Duration);
}

bool ASkill_Shield::CanExecute(const APawnMonster* Monster) const
{
	if (!Super::CanExecute(Monster))
		return false;
	//Check for all projectiles near this Pawn
	//If a hostile projectile, and its damage can kill us, allow execution of Shield skill
	for (AProjectile* Projectile : TActorRange<AProjectile>(Monster->GetWorld()))
	{
		if (Projectile->IsPlayerTeam() && !Monster->IsPlayerTeam() )	//This projectile is hostile
		{
			if ((Projectile->GetActorLocation() - Monster->GetActorLocation()).Size() <= SearchRadius )		//This projectile is within range
				return true;
		}
	}
	return false;
}

void ASkill_Shield::Execute()
{
	//Do not call Super!
	UE_LOG(LogTemp, Warning, TEXT("EXECUTING SHIELD"));
	
	APawnMonster* Monster = Cast<APawnMonster>(GetOwner());
	if (Monster == nullptr)
		return;
	Monster->SetDamageReduction(DamageReduction);	
}

void ASkill_Shield::Destroyed()
{
	Super::Destroyed();
	APawnMonster* Monster = Cast<APawnMonster>(GetOwner());
	if (Monster)
		Monster->SetDamageReduction(0.0);	//Reset damage reduction
}