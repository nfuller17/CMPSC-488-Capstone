// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill_Regenerate.h"
#include "PawnJerry.h"

void ASkill_Regenerate::BeginPlay()
{
	Super::BeginPlay();
	SetLifeSpan(Duration);
}

bool ASkill_Regenerate::CanExecute(const APawnMonster* Monster) const
{
	if (!Super::CanExecute(Monster))
		return false;
	if (!IsValid(Monster))
		return false;
	if (Monster->GetHealth() > MinHealth)
		return false;
	return true;
}

void ASkill_Regenerate::Execute()
{
	//Do not call Super!
	
	//Set timer
	GetWorldTimerManager().SetTimer(RegenerationTimer, this, &ASkill_Regenerate::AddHealth, RegenerationRate, true, 0);	
}

void ASkill_Regenerate::AddHealth()
{
	APawnMonster* Monster = Cast<APawnMonster>(GetOwner());
	if (IsValid(Monster))
	{
		if (Monster->IsDead())
		{
			GetWorldTimerManager().ClearTimer(RegenerationTimer);
			Destroy();
		}
		else
		{
			Monster->AddHealth(RegenerationAmount);
			return;
		}
	}

	APawnJerry* Player = Cast<APawnJerry>(GetOwner());
	if (IsValid(Player))
	{
		Player->AddHealth(RegenerationAmount);
	}
}