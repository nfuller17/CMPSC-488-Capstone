// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Projectile.h"
#include "Projectile_Singularity.generated.h"

//Child class of Projectile to special Singularity Cannon behavior

UCLASS()
class TOMANDJERRY_API AProjectile_Singularity : public AProjectile
{
	GENERATED_BODY()

public:
	FTimerHandle SingularityTimer;
	void Explode(AActor* TargetActor = nullptr) override;

protected:
	virtual void BeginPlay() override;
	void SpawnSingularity();
private:
	//Time in seconds to spawn the Singularity after this projectile is spawned
	UPROPERTY(EditAnywhere)
	float SingularitySpawnTime = 3.00;
	
};
