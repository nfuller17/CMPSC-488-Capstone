// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnMinion.h"
#include "PawnMonster.h"
#include "../TomAndJerryGameModeBase.h"

void APawnMinion::Died()
{
	Super::Died();
	ATomAndJerryGameModeBase* Game = Cast<ATomAndJerryGameModeBase>(GetWorld()->GetAuthGameMode());
	if (Game)
		Game->DecrementNumMonsters();
	Destroy();
}