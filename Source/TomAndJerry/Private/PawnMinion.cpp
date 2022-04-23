// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnMinion.h"
#include "PawnMonster.h"
#include "../TomAndJerryGameModeBase.h"

void APawnMinion::Died()
{
	Super::Died();
	ATomAndJerryGameModeBase* Game = Cast<ATomAndJerryGameModeBase>(GetWorld()->GetAuthGameMode());
	if (IsValid(Game))
		Game->DecrementNumMonsters();
	GetWorldTimerManager().SetTimer(DestroyTimer, this, &APawnMinion::DestroyHelper, DestroyDelay, false, DestroyDelay);
}