// Fill out your copyright notice in the Description page of Project Settings.


#include "Factory_Ally.h"

void AFactory_Ally::BeginPlay()
{
	Super::BeginPlay();
	SpawnAllyObjective();
}

//Only called when Pawn completes ally objective
void AFactory_Ally::RestartTimer()
{
	GetWorldTimerManager().SetTimer(SpawnTimer, this, &AFactory_Ally::SpawnAllyObjective, RespawnRate, false, RespawnRate);
}

void AFactory_Ally::SpawnAllyObjective()
{
	if (AllyObjective != nullptr)	//Already exists, don't spawn another
		return;
	if (AllyObjectives.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("ARRAY FOR ALLY FACTORY NOT SET!"))
		return;
	}
	AllyObjective = GetWorld()->SpawnActor<AObjectiveAlly>(AllyObjectives[FMath::RandRange(0, AllyObjectives.Num() - 1)], GetActorLocation(), GetActorForwardVector().Rotation());
	if (AllyObjective != nullptr)
	{
		AllyObjective->SetFactory(this);
	}
}