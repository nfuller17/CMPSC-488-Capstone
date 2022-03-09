// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/NavigationObjectBase.h"
#include "PawnMonster.h"
#include "Factory_Monster.generated.h"

class APawnMonster;

/**
 * 
 */
UCLASS()
class TOMANDJERRY_API AFactory_Monster : public ANavigationObjectBase
{
	GENERATED_BODY()
	
public:
	FTimerHandle SpawnTimer;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void SpawnMonster();
	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<APawnMonster>> Monsters;
	
private:
	UPROPERTY(EditAnywhere)
		float SpawnInterval;
	
};
