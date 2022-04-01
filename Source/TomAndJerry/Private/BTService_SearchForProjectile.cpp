// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_SearchForProjectile.h"
#include "EngineUtils.h"
#include "Kismet/KismetMathLibrary.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "PawnMonster.h"
#include "PawnJerry.h"

UBTService_SearchForProjectile::UBTService_SearchForProjectile()
{
	NodeName = TEXT("Search for Projectiles");
}

void UBTService_SearchForProjectile::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AAIController* AIController = OwnerComp.GetAIOwner();
	if (AIController == nullptr)
		return;

	APawnMonster* PawnOwner = Cast<APawnMonster>(AIController->GetPawn());
	if (PawnOwner == nullptr)
		return;

	bool bOnPlayerTeam = PawnOwner->IsPlayerTeam();
	AProjectile* TargetProj = nullptr;
	float ClosestDistance = ProjectileDetectionRadius;

	//Search for projectiles that we can dodge
	for (auto Proj : TActorRange<AProjectile>(AIController->GetWorld()))
	{
		//Simple intial check if we have a direct line of sight to the projectile
		if (!AIController->LineOfSightTo(Proj))
			continue;

		//Is this projectile hostile?
		APawnMonster* Monster = Cast<APawnMonster>(Proj->GetOwner());
		APawnJerry* Player = Cast<APawnJerry>(Proj->GetOwner());
		if (bOnPlayerTeam && Monster != nullptr && !Monster->IsPlayerTeam() ||
			!bOnPlayerTeam && (Player != nullptr || Monster != nullptr && Monster->IsPlayerTeam()))
		{
			//Can we see this projectile? Don't want to dodge a projectile coming from behind
			float Distance = 0.0;
			if (CanSeeTarget(PawnOwner, Proj, Distance) && ProjectileWillCollide(PawnOwner, Proj, DeltaSeconds))
			{
				//Get the projectile closest to us
				if (Distance < ClosestDistance)
				{
					ClosestDistance = Distance;
					TargetProj = Proj;
				}
			}
		}
	}
}

bool UBTService_SearchForProjectile::CanSeeTarget(APawn* OwnerPawn, AActor* TargetActor, float& Distance)
{
	//Get vector from this AI to target
	FVector DirectionToTarget = (TargetActor->GetActorLocation() - OwnerPawn->GetActorLocation());
	Distance = DirectionToTarget.Size();

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
bool UBTService_SearchForProjectile::ProjectileWillCollide(APawn* OwnerPawn, AProjectile* Proj, const float& DeltaSeconds)
{
	//Get time when x-component of OwnerPawn and Proj will be the same
	float xSeconds = (OwnerPawn->GetActorLocation().X - Proj->GetActorLocation().X) / (Proj->GetVelocity().X - OwnerPawn->GetActorLocation().X);
	if (xSeconds < 0)	//Will never cross
		return false;

	float ySeconds = (OwnerPawn->GetActorLocation().Y - Proj->GetActorLocation().Y) / (Proj->GetVelocity().Y - OwnerPawn->GetActorLocation().Y);
	if (ySeconds < 0)	//Will never cross
		return false;


	float zSeconds = (OwnerPawn->GetActorLocation().Z - Proj->GetActorLocation().Z) / (Proj->GetVelocity().Z - OwnerPawn->GetActorLocation().Z);
	if (zSeconds < 0)	//Will never cross
		return false;

	//Actor location returns a point that is probably in the center of the character capsule component
	//We want to return true if the projectile will hit any point on the capsule component
	float xLowerBound = xSeconds - 1.0;
	float xUpperBound = xSeconds + 1.0;
	if (ySeconds < xLowerBound || ySeconds > xUpperBound || zSeconds < xLowerBound || zSeconds > xUpperBound)
		return false;
	float yLowerBound = ySeconds - 1.0;
	float yUpperBound = ySeconds + 1.0;
	if (xSeconds < yLowerBound || xSeconds > yUpperBound || zSeconds < yLowerBound || zSeconds > yUpperBound)
		return false;
	float zLowerBound = zSeconds - 1.0;
	float zUpperBound = zSeconds + 1.0;
	return xSeconds >= zLowerBound && xSeconds <= zUpperBound && ySeconds >= zLowerBound && ySeconds <= zUpperBound;
}