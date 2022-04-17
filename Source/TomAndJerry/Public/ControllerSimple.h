// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "ControllerSimple.generated.h"

/**
 * 
 */
UCLASS()
class TOMANDJERRY_API AControllerSimple : public AAIController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
		class UBehaviorTree* AIBehavior;
	
};
