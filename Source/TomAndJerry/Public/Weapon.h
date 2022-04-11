// Fill out your copyright notice in the Description page of Project Settings.
// Base class for all weapon implementations

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.h"
#include "AmmoComponent.h"
#include "Weapon.generated.h"

class AProjectile;

UCLASS(abstract)
class TOMANDJERRY_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	uint8 GetWeaponNumber(){return WeaponNumber;}
	virtual void FirePrimary();
	void BeginFire();
	void StopFire();
	void EnableFire();
	void SetAmmoComponent(UAmmoComponent* _AmmoComponent){AmmoComponent = _AmmoComponent;}
	FTimerHandle FiringTimer;
	FTimerHandle SpamTimer;		//Independent timer to prevent spamming mouse button and bypassing fire rate

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	//WeaponNumber is a unique identifier for a weapon, used in tandem with input binding on keyboard
	UPROPERTY(EditDefaultsOnly, Category="Weapon Properties", meta=(ClampMin= "0"))
		uint8 WeaponNumber;
	//Ammo cost of this weapon, per fire. Different across weapons
	UPROPERTY(EditDefaultsOnly, Category="Ammo", meta=(ClampMin= "0"))
	uint16 AmmoPerFire = 1;
	
	

private:
	//If true, this weapon can damage the boss. Otherwise, does not damage the boss.
	UPROPERTY(EditAnywhere);
		USoundBase* firingSound;
	UPROPERTY(EditDefaultsOnly, Category = "Weapon Properties")
		bool IsSuperWeapon = false;
	UPROPERTY(EditDefaultsOnly, Category="Weapon Properties")
		float FireRate;		//Time, in seconds, between each shot
	UPROPERTY(VisibleAnywhere)
		USceneComponent* Root;
	UPROPERTY(VisibleAnywhere)
		USkeletalMeshComponent* Mesh;
	//How many times should this weapon fire, whether instant hit or projectile, per click.
	UPROPERTY(EditAnywhere, Category="Weapon Properties", meta=(ClampMin = "1"))
		uint8 NumFirePerClick = 1;
	//Aim offset. 0 to aim at player crosshair, 1 to aim with a randomly chosen vector
	UPROPERTY(EditAnywhere, Category="Weapon Properties", meta=(ClampMin = "0.0"))
		float Spread;
	UPROPERTY(EditAnywhere, Category="Weapon Properties")
		UParticleSystem* MuzzleFlash;
	UPROPERTY(EditDefaultsOnly, Category="HitScan Properties")
		bool bIsHitScan;		//Whether or not the weapon will fire a projectile
	UPROPERTY(EditAnywhere, Category="HitScan Properties", meta=(ClampMin = "0.0"))
		float MaxRange = 1000.f;
	UPROPERTY(EditAnywhere, Category="HitScan Properties", meta=(ClampMin = "0.0"))
		float HitscanDamage;
	UPROPERTY(EditAnywhere, Category="Hitscan Properties")
		UParticleSystem* ImpactEffect;
	UPROPERTY(EditDefaultsOnly, Category="Projectile Properties")
		TSubclassOf<AProjectile> ProjectileClass;
	bool bCanFire = true;
	UAmmoComponent* AmmoComponent;
};
