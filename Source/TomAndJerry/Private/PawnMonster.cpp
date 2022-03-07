// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnMonster.h"
#include "Components/SkeletalMeshComponent.h"
#include "Projectile.h"

// Sets default values
APawnMonster::APawnMonster()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void APawnMonster::BeginPlay()
{
	Super::BeginPlay();
	Health = HealthMax;
}

// Called every frame
void APawnMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APawnMonster::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void APawnMonster::FireProjectile()
{
	//Spawn a projectile, in front of the Monster, facing the direction of the Monster
	AController* MonsterController = GetController();
	if (MonsterController == nullptr)
		return;
	FVector SpawnLocation;
	FRotator SpawnRotation;
	MonsterController->GetPlayerViewPoint(SpawnLocation, SpawnRotation);
	
	SpawnLocation += FVector(0, 50, 0);		//Not sure which is "Forward"
	
	AProjectile* Proj = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, SpawnLocation, SpawnRotation);
	if (Proj == nullptr)
		return;
	Proj->SetOwner(this);
	UE_LOG(LogTemp, Warning, TEXT("Monster projecitle fired."));
}