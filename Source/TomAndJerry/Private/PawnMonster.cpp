// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnMonster.h"
#include "Components/SkeletalMeshComponent.h"
#include "Projectile.h"
#include "AIController.h"
#include "Components/CapsuleComponent.h"

// Sets default values
APawnMonster::APawnMonster()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void APawnMonster::BeginPlay()
{
	Super::BeginPlay();
	Health = HealthMax;
}

// Called every frame
void APawnMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APawnMonster::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float APawnMonster::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float DamageToDo = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	APawnMonster* OtherMonster = Cast<APawnMonster>(DamageCauser);
	if (OtherMonster)
	{
		//Friendly fire from another Monster - ignore damage
		//However, modify this section when friendly AI is implemented (which may be of type Monster)
		UE_LOG(LogTemp, Warning, TEXT("Friendly fire!"));
		return 0;
	}
	DamageToDo = FMath::Min(Health, DamageToDo);
	Health -= DamageToDo;
	if (IsDead())
	{
		Died();
	}
	return DamageToDo;
}

void APawnMonster::StartFire()
{
	if (bCanFire)
	{
		GetWorldTimerManager().SetTimer(FiringTimer, this, &APawnMonster::StopFire, FireRate, false, FireRate);	//Do not loop timer, as AI behavior tree will repeatedly call StartFire
		FireProjectile();
		bCanFire = false;
	}
}

void APawnMonster::StopFire()
{
	bCanFire = true;
	GetWorldTimerManager().ClearTimer(FiringTimer);
}

void APawnMonster::FireProjectile()
{
	//Spawn a projectile, in front of the Monster, facing the direction of the Monster
	AAIController* MonsterController = Cast<AAIController>(GetController());
	if (MonsterController == nullptr)
		return;
	APawn* TargetPawn = Cast<APawn>(MonsterController->GetFocusActor());
	if (TargetPawn == nullptr)
		return;
	FVector SpawnLocation = MonsterController->GetPawn()->GetActorLocation();
	SpawnLocation += FVector(0, 50, 0);		//Not sure which is "Forward"
	FRotator SpawnRotation = FVector(TargetPawn->GetActorLocation() - SpawnLocation).Rotation();
	FTransform Transform = FTransform(SpawnRotation, SpawnLocation, FVector(1,1,1));
	AProjectile* Proj = GetWorld()->SpawnActorDeferred<AProjectile>(ProjectileClass, Transform, this, this);
	if (Proj)
	{
		Proj->FinishSpawning(Transform);		
	}
}

bool APawnMonster::IsDead() const
{
	return Health <= 0;
}

void APawnMonster::Died()
{
	DetachFromControllerPendingDestroy();
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//Normally, we want to destroy the Actor here after dying
	//However, this is a virtual method that is overridden in derived classes
	//Calling Destroy here could be dangerous if it finishes first before the derived class can finish its implementation of this function
}