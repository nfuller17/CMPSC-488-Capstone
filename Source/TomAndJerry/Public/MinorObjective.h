// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MinorObjective.generated.h"

UCLASS()
class TOMANDJERRY_API AMinorObjective : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMinorObjective();
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	// Called when player stands on Minor Objective
	void BeginActivation();
	// Called when player walks off Minor Objective
	void StopActivation();
	// Called when player successfully completes Minor Objective. Set as virtual so child classes can override to provide their own rewards (weapons, allies, health, etc.)
	virtual void Complete();
	FTimerHandle ActivationTimer;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
	float CompletionTime = 10.0;	//How long, in seconds, the player must stand on a Minor Objective to complete it
};
