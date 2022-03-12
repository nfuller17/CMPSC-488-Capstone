// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PawnMonster.generated.h"

//Base class for AI enemies

class AProjectile;

UCLASS(abstract)
class TOMANDJERRY_API APawnMonster : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APawnMonster();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void SetHealth(uint32 NewHealth) { Health = NewHealth; }
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	void StartFire();
	void FireProjectile();
	void StopFire();
	FTimerHandle FiringTimer;
	
	UFUNCTION(BlueprintPure)
	bool IsDead() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(VisibleAnywhere)
		float Health;
	UPROPERTY(EditAnywhere)
		float HealthMax;


private:
	UPROPERTY(EditAnywhere, Category="Combat")
		bool bIsMeleeOnly;
	UPROPERTY(EditAnywhere)
	TSubclassOf<AProjectile> ProjectileClass;
	UPROPERTY(EditAnywhere, Category="Combat")
		float FireRate;
	bool bCanFire = true;
};
