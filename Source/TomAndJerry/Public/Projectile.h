// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "Projectile.generated.h"

//Projectile is an actor that moves in a fixed direction and speed
//When player fires weapon, the weapon will spawn the projectile at a location (muzzle) and rotation (player aim)
//We can call MoveComponent on the Root member of Projectile to make the Projectile move, and all attached components will follow
//Projectiles should damage enemy Pawns it hits (so it must have a Pawn or Controller owner)

UCLASS()
class TOMANDJERRY_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile();
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	virtual void Explode(AActor* TargetActor = nullptr);
	virtual float GetDamage() const{return Damage;}
	virtual void SetDamage(const float& _Damage){Damage = _Damage;}
	virtual bool IsPlayerTeam() const{return PlayerTeam;}
	virtual void SetTeam(const bool& bPlayerTeam) {PlayerTeam = bPlayerTeam;}
	void SetCanDamageBoss(const bool& _CanDamageBoss) { CanDamageBoss = _CanDamageBoss; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	bool PlayerTeam;
	
private:
	bool CanDamageBoss = false;
	UPROPERTY(VisibleAnywhere)
		USphereComponent* Root;
	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* Mesh;
	UPROPERTY(EditAnywhere)
		UParticleSystem* Effect;
	UPROPERTY(EditDefaultsOnly)
	float Damage;
	UPROPERTY(EditDefaultsOnly)
	float Speed;
	UPROPERTY(EditDefaultsOnly)
	bool bSplashDamage;

};
