// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectiveAlly.h"

void AObjectiveAlly::Complete()
{
	Super::Complete();
	if (ActivatingPawn != nullptr)
	{
		//Spawn a random ally from Allies list
		if (Allies.Num() == 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("ARRAY FOR OBJECTIVE ALLY NOT SET!"))
			return;
		}
		APawnAlly* Ally = GetWorld()->SpawnActor<APawnAlly>(Allies[FMath::RandRange(0, Allies.Num() - 1)], ActivatingPawn->GetActorLocation() + FVector(200, 0, 0), GetActorForwardVector().Rotation());
		if (Ally != nullptr)
		{
			Ally->SetTeam(true);
		}
	}
	if (Factory != nullptr)
	{
		Factory->RestartTimer();
		Factory->SetAllyObjective(nullptr);
	}
	Destroy();
}