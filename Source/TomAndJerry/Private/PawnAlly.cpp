// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnAlly.h"
#include "../TomAndJerryGameModeBase.h"

void APawnAlly::Died()
{
	Super::Died();
	ATomAndJerryGameModeBase* Game = Cast<ATomAndJerryGameModeBase>(GetWorld()->GetAuthGameMode());
	if (Game)
		Game->DecrementNumAlliesForSpectate();
	UE_LOG(LogTemp, Warning, TEXT("Ally died!"));
	GetWorldTimerManager().SetTimer(DestroyTimer, this, &APawnAlly::DestroyHelper, DestroyDelay, false, DestroyDelay);
}