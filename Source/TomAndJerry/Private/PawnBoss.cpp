// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnBoss.h"
#include "PawnMonster.h"
#include "../TomAndJerryGameModeBase.h"

void APawnBoss::Died()
{
	Super::Died();
	ATomAndJerryGameModeBase* Game = Cast<ATomAndJerryGameModeBase>(GetWorld()->GetAuthGameMode());
	if (Game)
		Game->EndGame(true);
	GetWorldTimerManager().SetTimer(DestroyTimer, this, &APawnBoss::DestroyHelper, DestroyDelay, false, DestroyDelay);
}