// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnAlly.h"
#include "../TomAndJerryGameModeBase.h"

void APawnAlly::Died()
{
	Super::Died();
	ATomAndJerryGameModeBase* Game = Cast<ATomAndJerryGameModeBase>(GetWorld()->GetAuthGameMode());
	if (IsValid(Game))
		Game->DecrementNumAlliesForSpectate();
	GetWorldTimerManager().SetTimer(DestroyTimer, this, &APawnAlly::DestroyHelper, DestroyDelay, false, DestroyDelay);
}