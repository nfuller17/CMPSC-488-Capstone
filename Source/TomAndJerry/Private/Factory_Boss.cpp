// Fill out your copyright notice in the Description page of Project Settings.


#include "Factory_Boss.h"

bool AFactory_Boss::SpawnBoss()
{
	UE_LOG(LogTemp, Warning, TEXT("Spawning boss"));
	//Pick a random monster from the Monsters array
	TSubclassOf<APawnBoss> BossClass = Bosses[FMath::RandRange(0, Bosses.Num()-1)];
	if (BossClass == nullptr)
		return false;
	return GetWorld()->SpawnActor<APawnBoss>(BossClass, GetActorLocation(), GetActorForwardVector().Rotation()) != nullptr;
}