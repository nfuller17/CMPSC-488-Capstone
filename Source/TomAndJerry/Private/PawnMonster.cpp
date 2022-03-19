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
	GetWorldTimerManager().SetTimer(EnergyTimer, this, &APawnMonster::AddEnergy, EnergyRegenRate, true, EnergyRegenRate);
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
	DamageToDo *= (1.0 - DamageReduction);	//For Shield skill
	DamageToDo = FMath::Min(Health, DamageToDo);
	Health -= DamageToDo;
	UE_LOG(LogTemp, Warning, TEXT("Monster health: %f"), Health);
	if (IsDead())
	{
		Died();
	}
	return DamageToDo;
}

void APawnMonster::AddHealth(const float& Amount)
{
	if (IsDead())
		return;
	Health += FMath::Min(Amount, HealthMax - Health);
	UE_LOG(LogTemp, Warning, TEXT("Regenerate: %f"), Health);
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
		Proj->SetTeam(false);
		Proj->FinishSpawning(Transform);		
	}
}

//Called by the AI Behavior Tree, which has made a check to see if this Pawn has the requested Skill
//This function will call CanExecute() on the requested Skill, which will return a boolean indiciating whether the correct conditions are in place
//If CanExecute() returns true, decrement this Pawn's energy by the Skill's cost
//This function is void, but the calling AI Behavior Tree will always return Succeed
void APawnMonster::DoSkill(const TSubclassOf<ASkill> RequestedSkill)
{
	if (bSkillIsActive)
		return;
	for (auto SkillBlueprintClass: Skills)
	{
		ASkill* SkillObject = SkillBlueprintClass->GetDefaultObject<ASkill>();
		if (SkillObject && SkillObject->IsA(RequestedSkill))
		{
			if (Energy < SkillObject->GetEnergyCost())
				return;
			if (!SkillObject->CanExecute(this))
				return;
			ASkill* SkillInstance = GetWorld()->SpawnActor<ASkill>(SkillObject->GetClass());
			if (SkillInstance != nullptr)
			{
				SkillInstance->SetOwner(this);
				SkillInstance->Execute();
				UE_LOG(LogTemp, Warning, TEXT("Energy before: %d"), Energy);
				Energy -= SkillInstance->GetEnergyCost();
				UE_LOG(LogTemp, Warning, TEXT("Energy after: %d"), Energy);
				bSkillIsActive = true;
				return;
			}
		}
	}

}

bool APawnMonster::HasSkill(TSubclassOf<ASkill> SkillClass)
{
	for (auto Skill : Skills)
	{
		ASkill* SkillObject = Skill->GetDefaultObject<ASkill>();
		if (SkillObject && SkillObject->IsA(SkillClass))
			return true;
	}
	return false;
}

void APawnMonster::AddEnergy()
{
	if (IsDead())
	{
		GetWorldTimerManager().ClearTimer(EnergyTimer);
		return;
	}
	if (Energy >= MaxEnergy)
		return;
	Energy += EnergyRegenAmount;
	if (Energy > MaxEnergy)
		Energy = MaxEnergy;
}

bool APawnMonster::IsDead() const
{
	return Health <= 0;
}

void APawnMonster::Died()
{
	StopFire();
	GetWorldTimerManager().ClearTimer(EnergyTimer);
	DetachFromControllerPendingDestroy();
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//Normally, we want to destroy the Actor here after dying
	//However, this is a virtual method that is overridden in derived classes
	//Calling Destroy here could be dangerous if it finishes first before the derived class can finish its implementation of this function
}