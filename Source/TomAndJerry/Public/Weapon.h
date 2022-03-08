// Fill out your copyright notice in the Description page of Project Settings.
// Base class for all weapon implementations

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.h"
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
	
	virtual uint8 GetWeaponNumber() PURE_VIRTUAL(AWeapon::GetWeaponNumber, return WeaponNumber;);
	virtual void FirePrimary();
	void BeginFire();
	void StopFire();
	void EnableFire();
	FTimerHandle FiringTimer;
	FTimerHandle SpamTimer;		//Independent timer to prevent spamming mouse button and bypassing fire rate

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	//WeaponNumber is a unique identifier for a weapon, used in tandem with input binding on keyboard
	UPROPERTY(EditDefaultsOnly, Category="Weapon Properties")
		uint8 WeaponNumber;

private:
	UPROPERTY(EditDefaultsOnly, Category="Weapon Properties")
		float FireRate;		//Time, in seconds, between each shot
	UPROPERTY(VisibleAnywhere)
		USceneComponent* Root;
	UPROPERTY(VisibleAnywhere)
		USkeletalMeshComponent* Mesh;
	UPROPERTY(EditAnywhere)
		UParticleSystem* MuzzleFlash;
	UPROPERTY(EditAnywhere)
		UParticleSystem* ImpactEffect;
	UPROPERTY(EditDefaultsOnly, Category="HitScan Properties")
		bool bIsHitScan;		//Whether or not the weapon will fire a projectile
	UPROPERTY(EditAnywhere, Category="HitScan Properties")
		float MaxRange = 1000.f;
	UPROPERTY(EditAnywhere, Category="HitScan Properties")
		float HitscanDamage;
	UPROPERTY(EditDefaultsOnly, Category="Projectile Properties")
		TSubclassOf<AProjectile> ProjectileClass;
	bool bCanFire = true;
};
