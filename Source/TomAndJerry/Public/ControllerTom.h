// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "ControllerTom.generated.h"

/**
 * Controller for AI
 */
UCLASS()
class TOMANDJERRY_API AControllerTom : public AAIController
{
	GENERATED_BODY()

public:
	virtual void Tick(float DeltaSeconds) override;

protected:
	virtual void BeginPlay() override;

private:
    UPROPERTY(EditAnywhere)
    class UBehaviorTree* AIBehavior;
};


