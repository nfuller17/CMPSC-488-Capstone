// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile_Singularity.h"

void AProjectile_Singularity::BeginPlay()
{
	Super::BeginPlay();
	GetWorldTimerManager().SetTimer(SingularityTimer, this, &AProjectile_Singularity::SpawnSingularity, SingularitySpawnTime, false, SingularitySpawnTime);
}

void AProjectile_Singularity::SpawnSingularity()
{
	//Spawn singularity

	//Destroy
	Destroy();
}

//Not calling Super here, since we really don't want to do any damage with this projectile
//Deal damage from the Singularity actor itself
void AProjectile_Singularity::Explode(AActor* TargetActor)
{
	GetWorldTimerManager().ClearTimer(SingularityTimer);
	SpawnSingularity();
}