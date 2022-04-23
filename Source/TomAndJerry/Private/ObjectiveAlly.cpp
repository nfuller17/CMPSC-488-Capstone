// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectiveAlly.h"

void AObjectiveAlly::Complete()
{
	Super::Complete();
	if (IsValid(ActivatingPawn))
	{
		//Spawn a random ally from Allies list
		if (Allies.Num() == 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("ARRAY FOR OBJECTIVE ALLY NOT SET!"))
			return;
		}
		APawnAlly* Ally = GetWorld()->SpawnActor<APawnAlly>(Allies[FMath::RandRange(0, Allies.Num() - 1)], ActivatingPawn->GetActorLocation() + FVector(200, 0, 0), GetActorForwardVector().Rotation());
		if (IsValid(Ally))
		{
			Ally->SetTeam(true);
		}
	}
	if (IsValid(Factory))
	{
		Factory->RestartTimer();
		Factory->SetAllyObjective(nullptr);
	}
	Destroy();
}

void AObjectiveAlly::Destroyed()
{
	Super::Destroyed();
	Allies.Empty();
}