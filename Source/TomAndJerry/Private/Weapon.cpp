// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"

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
	if (OwnerPawn)
	{
		AController* OwnerController = OwnerPawn->GetController();
		if (OwnerController)
		{
			FVector FireLocation;
			FRotator FireRotation;
			OwnerController->GetPlayerViewPoint(FireLocation, FireRotation);	//Parameters are passed by reference, so FireLocation and FireRotation will contain values of player view point
			
			//Handle hit scan firing
			if (bIsHitScan)
			{
				FVector EndLocation = FireLocation + FireRotation.Vector() * MaxRange;
				FHitResult Hit;
				bool bHitActor = GetWorld()->LineTraceSingleByChannel(Hit, FireLocation, EndLocation, ECollisionChannel::ECC_GameTraceChannel2);	//Trace from FireLocation, in direction of FireRotation, end at EndLocation
				if (bHitActor)
				{
					//Damage Actor
					FVector DirectionFromShot = -FireRotation.Vector();
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
				FVector SpawnLocation;
				FRotator SpawnRotation;
				OwnerController->GetPlayerViewPoint(SpawnLocation, SpawnRotation);	//Parameters pass by value, so location and rotation are updated
				FTransform Transform = FTransform(SpawnRotation, SpawnLocation, FVector(1,1,1));
				AProjectile* Proj = GetWorld()->SpawnActorDeferred<AProjectile>(ProjectileClass, Transform, Cast<AActor>(OwnerPawn), OwnerPawn);
				if (Proj)
				{
					Proj->FinishSpawning(Transform);					
				}
			}
		}
	}
	
	
	//Spawn muzzle flash
	UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, Mesh, TEXT("Muzzle"));
}

