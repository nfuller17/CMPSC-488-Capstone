// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PawnJerry.h"
#include "PawnBoss.h"

// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	if (IsValid(Mesh))
	{
		Mesh->SetupAttachment(Root);
	}
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

bool AWeapon::IsFiring() const
{
	return GetWorldTimerManager().IsTimerActive(FiringTimer);
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
	if (!IsValid(OwnerPawn))
		return;
	//Check if we are dead
	APawnJerry* JerryPawn = Cast<APawnJerry>(OwnerPawn);
	if (JerryPawn == nullptr)
		return;

	if (!IsValid(JerryPawn))
		return;

	if (JerryPawn->IsDead())
	{
		StopFire();
		return;
	}
	AController* OwnerController = OwnerPawn->GetController();
	if (OwnerController == nullptr || !IsValid(OwnerController))
		return;

	if (AmmoComponent == nullptr || !IsValid(AmmoComponent))
		return;
	

	if (AmmoPerFire > AmmoComponent->GetAmmoAmount())
	{
		return;	
	}

	FVector StartLocation;
	FRotator Direction;
	OwnerController->GetPlayerViewPoint(StartLocation, Direction);

	if (bIsMelee)
	{
		//Start a ray trace from weapon location, in direction of player aim
		StartLocation = GetActorLocation();		//Reset StartLocation to weapon's location, not camera's
		Trace(StartLocation, Direction.Vector(), OwnerPawn);
	}
	else
	{
		for (int x = 0; x < NumFirePerClick; x++)
		{
			//Randomize rotation if we have spread
			FVector RealFireRotation = Direction.Vector() + FMath::VRand() * FMath::FRandRange(0.f, 1.0) * Spread;
			if (bIsHitScan)
			{
				Trace(StartLocation, RealFireRotation, OwnerPawn);
			}
			else	//Handle Projectile firing
			{
				if (Mesh == nullptr)
					return;
				FVector SpawnLocation = Mesh->GetBoneLocation("Ammo", EBoneSpaces::WorldSpace);
				FTransform Transform = FTransform(RealFireRotation.Rotation(), SpawnLocation, FVector(1, 1, 1));
				AProjectile* Proj = GetWorld()->SpawnActorDeferred<AProjectile>(ProjectileClass, Transform, Cast<AActor>(OwnerPawn), OwnerPawn);
				if (Proj == nullptr || !IsValid(Proj))
					return;

				Proj->SetTeam(true);	//Weapons always belong to Player, so we always set this Projectile to player team
				if (IsSuperWeapon)
					Proj->SetCanDamageBoss(true);
				Proj->FinishSpawning(Transform);
			}
		}
	}
	

	UGameplayStatics::SpawnSoundAttached(firingSound, Mesh, TEXT("Muzzle"));


	AmmoComponent->ConsumeAmmo(AmmoPerFire);
	
	
	//Spawn muzzle flash
	UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, Mesh, TEXT("Muzzle"));
}

void AWeapon::Trace(const FVector& StartLocation, const FVector& Direction, APawn* OwnerPawn)
{
	FVector EndLocation = StartLocation + Direction * MaxRange;
	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(OwnerPawn);
	bool bHitActor = GetWorld()->LineTraceSingleByChannel(Hit, StartLocation, EndLocation, ECollisionChannel::ECC_GameTraceChannel2, Params);	//Trace from FireLocation, in direction of FireRotation, end at EndLocation
	if (bHitActor)
	{
		//Damage Actor
		FVector DirectionFromShot = -Direction;
		AActor* HitActor = Hit.GetActor();
		if (HitActor == nullptr || !IsValid(HitActor))
			return;

		APawnBoss* Boss = Cast<APawnBoss>(HitActor);
		if (Boss == nullptr || Boss != nullptr && IsValid(Boss) && IsSuperWeapon)
		{
			FPointDamageEvent DamageEvent(HitscanDamage, Hit, DirectionFromShot, nullptr);
			HitActor->TakeDamage(HitscanDamage, DamageEvent, OwnerPawn->GetController(), this);
		}

		//Impact
		if (ImpactEffect)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, Hit.Location, DirectionFromShot.Rotation());
		}
	}
}

