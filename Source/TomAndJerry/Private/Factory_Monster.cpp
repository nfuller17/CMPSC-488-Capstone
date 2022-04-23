// Fill out your copyright notice in the Description page of Project Settings.


#include "Factory_Monster.h"
#include "../TomAndJerryGameModeBase.h"

bool AFactory_Monster::SpawnMonster()
{
	if (Monsters.Num() <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("CAUTION: Monster Factory without an initialized array!"))
		return false;		
	}
	//Pick a random monster from the Monsters array
	TSubclassOf<APawnMonster> MonsterClass = Monsters[FMath::RandRange(0, Monsters.Num()-1)];
	if (MonsterClass == nullptr)
		return false;
	APawnMonster* Monster = GetWorld()->SpawnActor<APawnMonster>(MonsterClass, GetActorLocation(), GetActorForwardVector().Rotation());
	if (!IsValid(Monster))
		return false;
	Monster->SetTeam(false);
	return true;
}

void AFactory_Monster::Destroyed()
{
	Super::Destroyed();
	Monsters.Empty();
}