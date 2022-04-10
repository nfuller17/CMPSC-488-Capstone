// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Singularity.generated.h"

UCLASS()
class TOMANDJERRY_API ASingularity : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASingularity();
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	//Strength of gravitational pull
	UPROPERTY(EditAnywhere)
	float PullStrength = 5000.0;
	//Distance from which victims will start to get pulled toward Singularity
	UPROPERTY(EditAnywhere)
	float PullRadius = 2500;
	//Victim takes this amount of damage when it passes through singularity
	UPROPERTY(EditAnywhere)
	float Damage = 5000.0;
	UPROPERTY(EditAnywhere)
	UParticleSystem* Effect;

};
