// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnMonster.h"
#include "Components/SkeletalMeshComponent.h"
#include "../TomAndJerryGameModeBase.h"
#include "Projectile.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"
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
	//Check if we are already dead
	if (IsDead())
	{
		return DamageToDo;
	}
	//Check for Friendly fire
	if (IsValid(EventInstigator))		//Important to check not null in the event Instigator dies before TakeDamage is called, such as in projectiles
	{
		APawnMonster* Monster = Cast<APawnMonster>(EventInstigator->GetPawn());
		if (PlayerTeam)	//This monster is on player team - ignore damage caused by other friendly AI and by player
		{
			if (IsValid(Monster) && Monster->PlayerTeam)
			{
				return 0.0;
			}
			if (Cast<APlayerController>(EventInstigator) != nullptr)
			{
				return 0.0;
			}
		}
		else //On hostile monster team - ignore damage caused by other hostile monsters
		{
			if (IsValid(Monster) && !Monster->PlayerTeam)
			{
				return 0.0;
			}
		}
	}
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
	if (bIsMeleeOnly)
		return;
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
	if (IsDead())
		return;
	//Spawn a projectile, in front of the Monster, facing the direction of the Monster
	AAIController* MonsterController = Cast<AAIController>(GetController());
	if (!IsValid(MonsterController))
		return;
	APawn* TargetPawn = Cast<APawn>(MonsterController->GetFocusActor());
	if (!IsValid(TargetPawn))
		return;
	FVector SpawnLocation = MonsterController->GetPawn()->GetActorLocation();
	SpawnLocation += FVector(0, 50, 0);		//Not sure which is "Forward"
	FRotator SpawnRotation = FVector(TargetPawn->GetActorLocation() - SpawnLocation).Rotation();
	FTransform Transform = FTransform(SpawnRotation, SpawnLocation, FVector(1,1,1));
	AProjectile* Proj = GetWorld()->SpawnActorDeferred<AProjectile>(ProjectileClass, Transform, this, this);
	if (IsValid(Proj))
	{
		Proj->SetTeam(false);
		float AddDamage = Proj->GetDamage();	//Apply any damage bonus, if any
		AddDamage *= DamageBonus;
		Proj->SetDamage(AddDamage);
		Proj->FinishSpawning(Transform);		
	}
}

void APawnMonster::StartMelee(AActor* Victim)
{
	if (bCanMelee)
	{
		GetWorldTimerManager().SetTimer(MeleeTimer, this, &APawnMonster::StopMelee, MeleeAttackRate, false, MeleeAttackRate);	//Do not loop timer, as AI behavior tree will repeatedly call StartMelee
		MeleeAttack(Victim);
		bCanMelee = false;
	}
}

void APawnMonster::StopMelee()
{
	bCanMelee = true;
	GetWorldTimerManager().ClearTimer(MeleeTimer);
}

void APawnMonster::MeleeAttack(AActor* Victim)
{
	if (IsDead())
		return;
	if (!IsValid(Victim))
		return;
	FPointDamageEvent DamageEvent(MeleeDamage, FHitResult(), GetActorLocation() - Victim->GetActorLocation(), nullptr);
	Victim->TakeDamage(MeleeDamage, DamageEvent, GetController(), this);
}

bool APawnMonster::IsAttackingWithMelee() const
{
	return GetWorldTimerManager().IsTimerActive(MeleeTimer);
}

//Called by the AI Behavior Tree, which has made a check to see if this Pawn has the requested Skill
//This function will call CanExecute() on the requested Skill, which will return a boolean indiciating whether the correct conditions are in place
//If CanExecute() returns true, decrement this Pawn's energy by the Skill's cost
//This function is void, but the calling AI Behavior Tree will always return Succeed
void APawnMonster::DoSkill(const TSubclassOf<ASkill> RequestedSkill)
{
	if (IsDead())
		return;
	if (bSkillIsActive)
		return;
	for (auto SkillBlueprintClass: Skills)
	{
		if (!IsValid(SkillBlueprintClass))
			continue;
		ASkill* SkillObject = SkillBlueprintClass->GetDefaultObject<ASkill>();
		if (IsValid(SkillObject) && SkillObject->IsA(RequestedSkill))
		{
			if (Energy < SkillObject->GetEnergyCost())
				return;
			if (!SkillObject->CanExecute(this))
				return;
			ASkill* SkillInstance = GetWorld()->SpawnActor<ASkill>(SkillObject->GetClass());
			if (IsValid(SkillInstance))
			{
				SkillInstance->SetOwner(this);
				SkillInstance->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform);
				SkillInstance->Execute();
				Energy -= SkillInstance->GetEnergyCost();
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
		if (!IsValid(Skill))
			continue;
		ASkill* SkillObject = Skill->GetDefaultObject<ASkill>();
		if (IsValid(SkillObject) && SkillObject->IsA(SkillClass))
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
	Skills.Empty();
	StopFire();
	GetWorldTimerManager().ClearTimer(EnergyTimer);
	DetachFromControllerPendingDestroy();
	if (IsValid(GetCapsuleComponent()))
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	//Spawn muzzle flash
	if (DeathEffect != nullptr)
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DeathEffect, GetActorLocation(), GetActorRotation(), true);
	//Hide character mesh
	USkeletalMeshComponent* MonsterMesh = GetMesh();
	if (IsValid(MonsterMesh))
		MonsterMesh->SetVisibility(false, false);
	//Normally, we want to destroy the Actor here after dying
	//However, this is a virtual method that is overridden in derived classes
	//Calling Destroy here could be dangerous if it finishes first before the derived class can finish its implementation of this function
	//Let children call their own Destroy timer

}

void APawnMonster::DestroyHelper()
{
	GetWorldTimerManager().ClearTimer(DestroyTimer);
	Destroy();
}