// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Dodge.h"
#include "EngineUtils.h"
#include "Kismet/KismetMathLibrary.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "PawnMonster.h"
#include "PawnJerry.h"

UBTTask_Dodge::UBTTask_Dodge()
{
	NodeName = TEXT("Dodge Projectile");
}

EBTNodeResult::Type UBTTask_Dodge::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!IsValid(AIController))
		return EBTNodeResult::Failed;

	APawnMonster* PawnOwner = Cast<APawnMonster>(AIController->GetPawn());
	if (!IsValid(PawnOwner))
		return EBTNodeResult::Failed;

	bool bOnPlayerTeam = PawnOwner->IsPlayerTeam();

	//Search for projectiles that we can dodge
	for (auto Proj : TActorRange<AProjectile>(AIController->GetWorld()))
	{
		//Simple intial check if we have a direct line of sight to the projectile
		if (!AIController->LineOfSightTo(Proj))
			continue;

		//Is this projectile hostile?
		if (IsValid(Proj))
		{
			APawnMonster* Monster = Cast<APawnMonster>(Proj->GetOwner());
			if (Monster == PawnOwner)
				continue;	//Don't dodge our own projectiles!
			APawnJerry* Player = Cast<APawnJerry>(Proj->GetOwner());
			if (bOnPlayerTeam && IsValid(Monster) && !Monster->IsPlayerTeam() ||
				!bOnPlayerTeam && (IsValid(Player) || IsValid(Monster) && Monster->IsPlayerTeam()))
			{
				//Can we see this projectile, and will this projectile potentially hit us?
				if (CanSeeTarget(PawnOwner, Proj) && ProjectileWillCollide(PawnOwner, Proj))
				{
					//There's at least one projectile that can hit us, dodge!
					FVector DodgeDirection = PawnOwner->GetActorRightVector();
					if (FMath::FRandRange(0.0, 1.0) <= 0.49)
						DodgeDirection = -DodgeDirection;
					FVector DodgeVector = (DodgeDirection * DodgeStrength) + FVector(0, 0, ZAdd);
					PawnOwner->LaunchCharacter(DodgeVector, true, true);
					return EBTNodeResult::Succeeded;
				}
			}
		}
	}
	return EBTNodeResult::Succeeded;
}

bool UBTTask_Dodge::CanSeeTarget(APawn* OwnerPawn, AActor* TargetActor)
{
	if (!IsValid(OwnerPawn) || !IsValid(TargetActor))
		return false;
	//Get vector from this AI to target
	FVector DirectionToTarget = (TargetActor->GetActorLocation() - OwnerPawn->GetActorLocation());
	float Distance = DirectionToTarget.Size();

	//Is Target within range?
	if (Distance > ProjectileDetectionRadius)
		return false;	//Target too far away

	//Get OwnerPawn's viewpoint
	FVector Location;
	FRotator Rotation;
	if (IsValid(OwnerPawn->GetController()))
		OwnerPawn->GetController()->GetPlayerViewPoint(Location, Rotation);

	//Turn OwnerPawn's rotation into normalized vector
	FVector ForwardVector = Rotation.Vector().GetSafeNormal(1.0);

	//Normalize the vector
	DirectionToTarget = DirectionToTarget.GetSafeNormal(1.0);

	//Get Dot Product
	float Dot = FVector::DotProduct(ForwardVector, DirectionToTarget);

	//Get angle between this AI's forward vector and the vector to target
	float Angle = UKismetMathLibrary::Acos(Dot) * (180 / PI);

	//If Angle < AngleOfSight, then TargetActor is in this AI's field of vision
	return Angle < AngleOfSight;
}

//Determine whether the projectile will collide with AI pawn (or will be in close range)
bool UBTTask_Dodge::ProjectileWillCollide(APawn* OwnerPawn, AActor* Proj)
{
	if (!IsValid(OwnerPawn) || !IsValid(Proj))
		return false;
	//Starting Location of AI + Velocity of AI * t = Starting Location of Projectile + Velocity of Projectile * t
	for (float x = 1.0; x <= 5.0; x++)
	{
		FVector NextLocation = Proj->GetActorLocation() + Proj->GetVelocity() * x;
		FVector Line = NextLocation - OwnerPawn->GetActorLocation();
		float Distance = Line.Size();
		if (Distance <= ProjectileDodgeRadius)
			return true;
	}
	return false;
}