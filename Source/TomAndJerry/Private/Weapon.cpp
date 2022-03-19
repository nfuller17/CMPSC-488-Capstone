// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PawnJerry.h"

// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//Turn on the timer, handled by FiringTimer, to begin firing for as long as player holds down mouse button
void AWeapon::BeginFire()
{
	if (bCanFire)
	{
		bCanFire = false;
		GetWorldTimerManager().SetTimer(SpamTimer, this, &AWeapon::EnableFire, FireRate, false, FireRate);
		GetWorldTimerManager().SetTimer(FiringTimer, this, &AWeapon::FirePrimary, FireRate, true, 0);
	}
}

//Stop timer, handled by FiringTimer, to stop firing as soon as player releases mouse button
void AWeapon::StopFire()
{
	GetWorldTimerManager().ClearTimer(FiringTimer);
}

void AWeapon::EnableFire()
{
	bCanFire = true;
	GetWorldTimerManager().ClearTimer(SpamTimer);
}

//Mechanics for firing a weapon:
//Set a start location to begin a trace
//Set the rotation of the trace
//If the weapon is hitscan, do a simple ray trace
//If the weapon is projectile, spawn a projectile
void AWeapon::FirePrimary()
{	
	//Get location and rotation to begin trace
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (OwnerPawn == nullptr)
		return;
	//Check if we are dead
	APawnJerry* JerryPawn = Cast<APawnJerry>(OwnerPawn);
	if (JerryPawn == nullptr)
		return;

	if (JerryPawn->GetHealth() <= 0)
	{
		StopFire();
		return;
	}
	AController* OwnerController = OwnerPawn->GetController();
	if (OwnerController == nullptr)
		return;
	
	FVector FireLocation;
	FRotator FireRotation;
	OwnerController->GetPlayerViewPoint(FireLocation, FireRotation);	//Parameters are passed by reference, so FireLocation and FireRotation will contain values of player view point
	
	for (int x = 0; x < NumFirePerClick; x++)
	{
		//Randomize rotation if we have spread
		FVector RealFireRotation = FireRotation.Vector() + FMath::VRand() * FMath::FRandRange(0.f, 1.0) * Spread;
		//Handle hit scan firing
		if (bIsHitScan)
		{
			FVector EndLocation = FireLocation + RealFireRotation * MaxRange;
			FHitResult Hit;
			FCollisionQueryParams Params;
			Params.AddIgnoredActor(this);
			Params.AddIgnoredActor(OwnerPawn);
			bool bHitActor = GetWorld()->LineTraceSingleByChannel(Hit, FireLocation, EndLocation, ECollisionChannel::ECC_GameTraceChannel2, Params);	//Trace from FireLocation, in direction of FireRotation, end at EndLocation
			if (bHitActor)
			{
				//Damage Actor
				FVector DirectionFromShot = -RealFireRotation;
				AActor* HitActor = Hit.GetActor();
				if (HitActor)
				{
					FPointDamageEvent DamageEvent(HitscanDamage, Hit, DirectionFromShot, nullptr);
					HitActor->TakeDamage(HitscanDamage, DamageEvent, OwnerController, this);
				}
				
				//Impact
				if (ImpactEffect)
				{
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, Hit.Location, DirectionFromShot.Rotation());
				}
			}						
		}
		else	//Handle Projectile firing
		{
			if (Mesh == nullptr)
				return;
			FVector SpawnLocation =  Mesh->GetBoneLocation("Ammo", EBoneSpaces::WorldSpace);
			FTransform Transform = FTransform(RealFireRotation.Rotation(), SpawnLocation, FVector(1,1,1));
			AProjectile* Proj = GetWorld()->SpawnActorDeferred<AProjectile>(ProjectileClass, Transform, Cast<AActor>(OwnerPawn), OwnerPawn);
			if (Proj)
			{
				Proj->SetTeam(true);	//Weapons always belong to Player, so we always set this Projectile to player team
				Proj->FinishSpawning(Transform);					
			}					
		}				
	}
	
	
	//Spawn muzzle flash
	UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, Mesh, TEXT("Muzzle"));
}

