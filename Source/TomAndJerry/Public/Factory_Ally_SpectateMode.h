// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/NavigationObjectBase.h"
#include "PawnAlly.h"
#include "Factory_Ally_SpectateMode.generated.h"

class APawnAlly;

UCLASS()
class TOMANDJERRY_API AFactory_Ally_SpectateMode : public ANavigationObjectBase
{
	GENERATED_BODY()

public:
	bool SpawnAlly();
protected:
	UPROPERTY(EditAnywhere)
		TArray<TSubclassOf<APawnAlly>> Allies;
};
