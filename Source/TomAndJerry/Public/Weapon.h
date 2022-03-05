// Fill out your copyright notice in the Description page of Project Settings.
// Base class for all weapon implementations

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

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

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	//WeaponNumber is a unique identifier for a weapon, used in tandem with input binding on keyboard
	//Set in C++, and should not be changed
	const uint8 WeaponNumber = 0;
	
	UPROPERTY(VisibleAnywhere)
		USceneComponent* Root;
	UPROPERTY(VisibleAnywhere)
		USkeletalMeshComponent* Mesh;
	UPROPERTY(EditAnywhere)
		UParticleSystem* MuzzleFlash;
	UPROPERTY(EditAnywhere)
		UParticleSystem* ImpactEffect;
	UPROPERTY(EditAnywhere)
		float MaxRange = 1000.f;
};
