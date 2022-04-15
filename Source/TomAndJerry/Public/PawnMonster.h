// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Skill.h"
#include "PawnMonster.generated.h"

//Base class for AI enemies

class AProjectile;
class ASkill;

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
	void SetHealth(uint32 NewHealth) { Health = NewHealth; }
	float GetHealth() const{return Health;}
	float GetHealthMax() const{return HealthMax;}
	void AddHealth(const float& Amount);
	void SetTeam(const bool& OnPlayerTeam){PlayerTeam = OnPlayerTeam;}
	bool IsPlayerTeam() const{return PlayerTeam;}
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	void StartFire();
	void FireProjectile();
	void StopFire();
	void StartMelee(AActor* Victim);
	void MeleeAttack(AActor* Victim);
	void StopMelee();
	uint8 GetEnergy(){return Energy;}
	void AddEnergy();
	void DoSkill(const TSubclassOf<ASkill> SkillClass);
	void SetSkillIsActive(const bool& IsActive){bSkillIsActive = IsActive;}
	virtual bool HasSkill(TSubclassOf<ASkill> SkillClass);
	virtual float GetDamageReduction() const{return DamageReduction;}
	virtual void SetDamageReduction(const float& _DamageReduction){DamageReduction = _DamageReduction;}
	virtual float GetDamageBonus() const{return DamageBonus;}
	virtual void SetDamageBonus(const float& _DamageBonus){DamageBonus = _DamageBonus;}
	bool IsMelee() const { return bIsMeleeOnly; }
	FTimerHandle FiringTimer;
	FTimerHandle MeleeTimer;
	FTimerHandle DestroyTimer;
	FTimerHandle EnergyTimer;
	
	UFUNCTION(BlueprintPure)
	bool IsDead() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Died();
	//Called in conjunction with DestroyTimer
	//Needed since callback function expects void type, but Destroy() returns boolean
	void DestroyHelper(){Destroy();}
	bool PlayerTeam;
	UPROPERTY(VisibleAnywhere)
		float Health;
	UPROPERTY(EditAnywhere)
		float HealthMax;
	//How long after dying should this Monster be destroyed
	UPROPERTY(EditDefaultsOnly, meta=(ClampMin = "0.0"))
		float DestroyDelay = 5.00;
	//The amount of energy required to perform a skill
	UPROPERTY(EditAnywhere, Category="Skills")
		uint8 MaxEnergy = 100;
	uint8 Energy = 0;
	//Interval in seconds to regenerate energy
	UPROPERTY(EditAnywhere, Category="Skills")
		float EnergyRegenRate = 1.0;
	//Amount on each interval to regenerate energy
	UPROPERTY(EditAnywhere, Category="Skills")
		uint8 EnergyRegenAmount = 1;
	//The allowed skills this Pawn can perform
	UPROPERTY(EditDefaultsOnly, Category="Skills")
	TArray<TSubclassOf<ASkill>> Skills;
	bool bSkillIsActive = false;
	float DamageReduction = 0.0;	//0 for no DamageReduction, 1 for 100% Damage Reduction (0 damage)
	float DamageBonus = 1.0;		//1 for no DamageBonus, >1 to apply extra damage


private:
	//Check to disable projectile firing
	UPROPERTY(EditAnywhere, Category="Combat")
		bool bIsMeleeOnly;
	//Melee damage amount
	UPROPERTY(EditAnywhere, Category="Combat")
	float MeleeDamage = 10.0;
	//How quickly in seconds this Pawn can attack a target with melee
	UPROPERTY(EditAnywhere, Category = "Combat")
	float MeleeAttackRate = 1.0;
	//Type of projectile to fire
	UPROPERTY(EditAnywhere, Category="Combat")
	TSubclassOf<AProjectile> ProjectileClass;
	//If projectile enabled, time in seconds between each fire
	UPROPERTY(EditAnywhere, Category="Combat")
		float FireRate;
	bool bCanFire = true;	//Used with timer
	bool bCanMelee = true;	//Used with timer
	UPROPERTY(EditAnywhere)
		UParticleSystem* DeathEffect;
};
