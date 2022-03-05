// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

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
			FVector EndLocation = FireLocation + FireRotation.Vector() * MaxRange;
			FHitResult Hit;
			bool bHitActor = GetWorld()->LineTraceSingleByChannel(Hit, FireLocation, EndLocation, ECollisionChannel::ECC_GameTraceChannel2);	//Trace from FireLocation, in direction of FireRotation, end at EndLocation
			if (bHitActor)
			{
				if (ImpactEffect)
				{
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, Hit.Location, (-FireRotation.Vector()).Rotation());
				}
			}
		}
	}
	
	
	//Spawn muzzle flash
	UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, Mesh, TEXT("Muzzle"));
}

