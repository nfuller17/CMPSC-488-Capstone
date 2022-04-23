// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill_DamageBoost.h"
#include "AIController.h"
#include "PawnMonster.h"
#include "PawnJerry.h"

void ASkill_DamageBoost::BeginPlay()
{
	Super::BeginPlay();
	SetLifeSpan(Duration);
}

bool ASkill_DamageBoost::CanExecute(const APawnMonster* Monster) const
{
	if (!Super::CanExecute(Monster))
		return false;
	//Execute if Enemy target has relatively high health
	AAIController* AI = Cast<AAIController>(Monster->GetController());
	if (IsValid(AI))
	{
		AActor* EnemyActor = AI->GetFocusActor();
		APawnJerry* Player = Cast<APawnJerry>(EnemyActor);
		if (!IsValid(Player))	//Try for AI on player team
		{
			APawnMonster* OtherMonster = Cast<APawnMonster>(EnemyActor);
			if (!IsValid(OtherMonster))
			{
				return false;
			}
			else
			{
				return OtherMonster->GetHealth() >= OtherMonster->GetHealthMax()*HealthPercentToExecute;
			}
		}
		else
		{
			return Player->GetHealth() >= Player->GetHealthMax()*HealthPercentToExecute;
		}
	}
	return false;
}

void ASkill_DamageBoost::Execute()
{
	APawnMonster* Monster = Cast<APawnMonster>(GetOwner());
	if (!IsValid(Monster))
		return;
	Monster->SetDamageBonus(DamageBonus);
}

void ASkill_DamageBoost::Destroyed()
{
	Super::Destroyed();
	APawnMonster* Monster = Cast<APawnMonster>(GetOwner());
	if (IsValid(Monster))
		Monster->SetDamageBonus(1.0);	//Reset damage bonus
}