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
	if (AIController == nullptr)
		return EBTNodeResult::Failed;

	APawnMonster* PawnOwner = Cast<APawnMonster>(AIController->GetPawn());
	if (PawnOwner == nullptr)
		return EBTNodeResult::Failed;

	bool bOnPlayerTeam = PawnOwner->IsPlayerTeam();

	//Search for projectiles that we can dodge
	for (auto Proj : TActorRange<AProjectile>(AIController->GetWorld()))
	{
		//Simple intial check if we have a direct line of sight to the projectile
		if (!AIController->LineOfSightTo(Proj))
			continue;

		//Is this projectile hostile?
		APawnMonster* Monster = Cast<APawnMonster>(Proj->GetOwner());
		if (Monster == PawnOwner)
			continue;	//Don't dodge our own projectiles!
		APawnJerry* Player = Cast<APawnJerry>(Proj->GetOwner());
		if (bOnPlayerTeam && Monster != nullptr && !Monster->IsPlayerTeam() ||
			!bOnPlayerTeam && (Player != nullptr || Monster != nullptr && Monster->IsPlayerTeam()))
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
	return EBTNodeResult::Succeeded;
}

bool UBTTask_Dodge::CanSeeTarget(APawn* OwnerPawn, AActor* TargetActor)
{
	//Get vector from this AI to target
	FVector DirectionToTarget = (TargetActor->GetActorLocation() - OwnerPawn->GetActorLocation());
	float Distance = DirectionToTarget.Size();

	//Is Target within range?
	if (Distance > ProjectileDetectionRadius)
		return false;	//Target too far away

	//Get OwnerPawn's viewpoint
	FVector Location;
	FRotator Rotation;
	if (OwnerPawn->GetController() != nullptr)
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

	/*
	float xSeconds = (OwnerPawn->GetActorLocation().X - Proj->GetActorLocation().X) / (Proj->GetVelocity().X - OwnerPawn->GetVelocity().X);
	if (xSeconds < 0)	//Will never cross
		return false;

	float ySeconds = (OwnerPawn->GetActorLocation().Y - Proj->GetActorLocation().Y) / (Proj->GetVelocity().Y - OwnerPawn->GetVelocity().Y);
	if (ySeconds < 0)	//Will never cross
		return false;


	float zSeconds = (OwnerPawn->GetActorLocation().Z - Proj->GetActorLocation().Z) / (Proj->GetVelocity().Z - OwnerPawn->GetVelocity().Z);
	if (zSeconds < 0)	//Will never cross
		return false;

	//Actor location returns a point that is probably in the center of the character capsule component
	//We want to return true if the projectile will hit any point on the capsule component
	float xLowerBound = xSeconds - 2.0;
	float xUpperBound = xSeconds + 2.0;
	if (ySeconds < xLowerBound || ySeconds > xUpperBound || zSeconds < xLowerBound || zSeconds > xUpperBound)
		return false;
	float yLowerBound = ySeconds - 2.0;
	float yUpperBound = ySeconds + 2.0;
	if (xSeconds < yLowerBound || xSeconds > yUpperBound || zSeconds < yLowerBound || zSeconds > yUpperBound)
		return false;
	float zLowerBound = zSeconds - 2.0;
	float zUpperBound = zSeconds + 2.0;
	return xSeconds >= zLowerBound && xSeconds <= zUpperBound && ySeconds >= zLowerBound && ySeconds <= zUpperBound;
	*/
}