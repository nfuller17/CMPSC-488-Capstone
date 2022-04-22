// Fill out your copyright notice in the Description page of Project Settings.


#include "Factory_Boss.h"

bool AFactory_Boss::SpawnBoss()
{
	//Pick a random monster from the Monsters array
	if (Bosses.Num() == 0)
	{
		return false;
	}
	TSubclassOf<APawnBoss> BossClass = Bosses[FMath::RandRange(0, Bosses.Num()-1)];
	if (BossClass == nullptr)
		return false;
	APawnBoss* Boss = GetWorld()->SpawnActor<APawnBoss>(BossClass, GetActorLocation(), GetActorForwardVector().Rotation());
	if (Boss == nullptr)
		return false;
	Boss->SetTeam(false);
	UE_LOG(LogTemp, Warning, TEXT("Boss spawned"));
	return true;
}

void AFactory_Boss::Destroyed()
{
	Super::Destroyed();
	Bosses.Empty();
}