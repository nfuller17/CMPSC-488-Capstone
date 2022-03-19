// Fill out your copyright notice in the Description page of Project Settings.


#include "Factory_Monster.h"

bool AFactory_Monster::SpawnMonster()
{
	if (Monsters.Num() <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("CAUTION: Monster Factory without an initialized array!"))
		return false;		
	}
	UE_LOG(LogTemp, Warning, TEXT("Spawning monster"));
	//Pick a random monster from the Monsters array
	TSubclassOf<APawnMonster> MonsterClass = Monsters[FMath::RandRange(0, Monsters.Num()-1)];
	if (MonsterClass == nullptr)
		return false;
	APawnMonster* Monster = GetWorld()->SpawnActor<APawnMonster>(MonsterClass, GetActorLocation(), GetActorForwardVector().Rotation());
	if (Monster == nullptr)
		return false;
	Monster->SetTeam(false);
	return true;
}