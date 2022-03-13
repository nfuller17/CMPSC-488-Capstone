// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/NavigationObjectBase.h"
#include "PawnBoss.h"
#include "Factory_Boss.generated.h"

class APawnBoss;

UCLASS()
class TOMANDJERRY_API AFactory_Boss : public ANavigationObjectBase
{
	GENERATED_BODY()
	
public:
	bool SpawnBoss();
	
protected:
	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<APawnBoss>> Bosses;
	
};
