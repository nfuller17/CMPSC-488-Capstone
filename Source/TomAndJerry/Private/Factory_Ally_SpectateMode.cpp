// Fill out your copyright notice in the Description page of Project Settings.


#include "Factory_Ally_SpectateMode.h"
#include "../TomAndJerryGameModeBase.h"

bool AFactory_Ally_SpectateMode::SpawnAlly()
{
	if (Allies.Num() <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("CAUTION: Ally Spectate Factory without an initialized array!"))
			return false;
	}

	TSubclassOf<APawnAlly> AllyClass = Allies[FMath::RandRange(0, Allies.Num() - 1)];
	if (AllyClass == nullptr)
		return false;
	APawnAlly* Ally = GetWorld()->SpawnActor<APawnAlly>(AllyClass, GetActorLocation(), GetActorForwardVector().Rotation());
	if (Ally == nullptr)
		return false;
	Ally->SetTeam(true);
	//ATomAndJerryGameModeBase* Game = Cast<ATomAndJerryGameModeBase>(GetWorld()->GetAuthGameMode());
	//if (Game != nullptr && Game->GetSpectateMode())
	//{
	//	Game->SpectateList.Emplace(Ally);
	//}
	return true;
}