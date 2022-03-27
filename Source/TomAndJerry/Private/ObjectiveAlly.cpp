// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectiveAlly.h"

void AObjectiveAlly::Complete()
{
	Super::Complete();
	if (ActivatingPawn != nullptr)
	{
		//Spawn a random ally from Allies list
		APawnAlly* Ally = GetWorld()->SpawnActor<APawnAlly>(Allies[FMath::RandRange(0, Allies.Num() - 1)], ActivatingPawn->GetActorLocation() + FVector(100, 0, 0), GetActorForwardVector().Rotation());
		if (Ally != nullptr)
		{
			Ally->SetTeam(true);
		}
	}
}