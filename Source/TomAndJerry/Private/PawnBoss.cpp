// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnBoss.h"
#include "PawnMonster.h"
#include "../TomAndJerryGameModeBase.h"

float APawnBoss::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float DamageToApply = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	APawnMonster* Monster = Cast<APawnMonster>(DamageCauser);
	if (Monster != nullptr && !Monster->IsPlayerTeam())
		return 0;
	return DamageToApply;
}

void APawnBoss::Died()
{
	Super::Died();
	ATomAndJerryGameModeBase* Game = Cast<ATomAndJerryGameModeBase>(GetWorld()->GetAuthGameMode());
	if (Game)
		Game->EndGame(true);
	GetWorldTimerManager().SetTimer(DestroyTimer, this, &APawnBoss::DestroyHelper, DestroyDelay, false, DestroyDelay);
}