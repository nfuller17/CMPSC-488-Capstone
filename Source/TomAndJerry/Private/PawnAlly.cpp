// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnAlly.h"

void APawnAlly::Died()
{
	Super::Died();
	GetWorldTimerManager().SetTimer(DestroyTimer, this, &APawnAlly::DestroyHelper, DestroyDelay, false, DestroyDelay);
}