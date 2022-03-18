// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill_Regenerate.h"

void ASkill_Regenerate::BeginPlay()
{
	Super::BeginPlay();
	SetLifeSpan(Duration);
}

bool ASkill_Regenerate::CanExecute(APawnMonster* Monster)
{
	if (!Super::CanExecute(Monster))
		return false;
	if (Monster->GetHealth() > MinHealth)
		return false;
	return true;
}

void ASkill_Regenerate::Execute()
{
	GetWorldTimerManager().SetTimer(RegenerationTimer, this, &ASkill_Regenerate::AddHealth, RegenerationRate, true, 0);	
}

void ASkill_Regenerate::AddHealth()
{
	APawnMonster* Monster = Cast<APawnMonster>(GetOwner());
	if (Monster == nullptr)
		return;
	Monster->AddHealth(RegenerationAmount);
}

void ASkill_Regenerate::Destroyed()
{
	Super::Destroyed();
	UE_LOG(LogTemp, Warning, TEXT("DESTROY!"));
}