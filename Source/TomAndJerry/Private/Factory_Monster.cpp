// Fill out your copyright notice in the Description page of Project Settings.


#include "Factory_Monster.h"

void AFactory_Monster::BeginPlay()
{
	Super::BeginPlay();
	GetWorldTimerManager().SetTimer(SpawnTimer, this, &AFactory_Monster::SpawnMonster, SpawnInterval, true, 0);
}

void AFactory_Monster::SpawnMonster()
{
	UE_LOG(LogTemp, Warning, TEXT("Spawning monster"));
	//Pick a random monster from the Monsters array
	TSubclassOf<APawnMonster> MonsterClass = Monsters[FMath::RandRange(0, Monsters.Num()-1)];
	if (MonsterClass == nullptr)
		return;
	GetWorld()->SpawnActor<APawnMonster>(MonsterClass, GetActorLocation(), GetActorForwardVector().Rotation());
}