// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/NavigationObjectBase.h"
#include "NOB_TwoPoints.generated.h"

/**
 * 
 */
UCLASS()
class TOMANDJERRY_API ANOB_TwoPoints : public ANavigationObjectBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
		bool bStart = true;
};
