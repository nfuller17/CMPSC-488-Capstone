// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickup_Health.h"
#include "PawnJerry.h"
#include "PawnMonster.h"

void APickup_Health::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	if (OtherActor != nullptr)
	{
		APawnJerry* Player = Cast<APawnJerry>(OtherActor);
		if (Player != nullptr)
		{
			if (Player->GetHealth() < Player->GetHealthMax())
			{
				Player->AddHealth(HealthAmount);
				Destroy();	
			}
		}
		else
		{
			APawnMonster* Monster = Cast<APawnMonster>(OtherActor);
			if (Monster != nullptr)
			{
				if (Monster->GetHealth() < Monster->GetHealthMax())
				{
					Monster->AddHealth(HealthAmount);
					Destroy();					
				}
			}
		}
	}
}