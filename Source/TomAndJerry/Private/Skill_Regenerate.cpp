// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill_Regenerate.h"
#include "PawnMonster.h"

bool ASkill_Regenerate::Execute()
{
	if (GetWorldTimerManager().IsTimerActive(RegenerationTimer))
		return false;
	APawnMonster* Monster = Cast<APawnMonster>(GetOwner());
	if (Monster == nullptr)
		return false;
	if (Monster->GetHealth() > MinHealth)
		return false;
	UE_LOG(LogTemp, Warning, TEXT("REGENERATE!"));
	GetWorldTimerManager().SetTimer(ExecutionTimer, this, &ASkill_Regenerate::StopExecution, Duration, false, Duration);
	GetWorldTimerManager().SetTimer(RegenerationTimer, this, &ASkill_Regenerate::AddHealth, RegenerationRate, true, 0);
	return true;
}

void ASkill_Regenerate::StopExecution()
{
	GetWorldTimerManager().ClearTimer(ExecutionTimer);
	GetWorldTimerManager().ClearTimer(RegenerationTimer);
}

void ASkill_Regenerate::AddHealth()
{
	APawnMonster* Monster = Cast<APawnMonster>(GetOwner());
	if (Monster == nullptr)
		return;
	Monster->AddHealth(RegenerationAmount);
}