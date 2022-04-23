// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_SetRandomLocation.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "AIController.h"
#include "NavigationSystem.h"


UBTService_SetRandomLocation::UBTService_SetRandomLocation()
{
	NodeName = TEXT("Set Random Location");
}

void UBTService_SetRandomLocation::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	//AI should try and roam the map
	//Choose a random location that is far enough, or a location that is out of sight

	AAIController* AI = OwnerComp.GetAIOwner();
	if (!IsValid(AI))
		return;
	APawn* AIPawn = AI->GetPawn();
	if (!IsValid(AIPawn))
		return;
	FVector CurrentLocation = AIPawn->GetActorLocation();
	FNavLocation NewLocation = FNavLocation();

	//First, see if we can find a valid point nearby us
	//Move there if the AI does not have a line of sight to it (i.e. in a building, behind a wall, etc.)
	UNavigationSystemV1* Navigation = Cast<UNavigationSystemV1>(GetWorld()->GetNavigationSystem());
	if (!IsValid(Navigation))
		return;

	uint8 SafeCount = 0;
	while (true)
	{
		if (Navigation->GetRandomReachablePointInRadius(CurrentLocation, DistanceToRoam, NewLocation))
		{
			if ((CurrentLocation - NewLocation.Location).Size() < MinimumDistance)
				continue;
			//Check if AI can see this location
			if (IsValid(OwnerComp.GetBlackboardComponent()))
			{
				if (CanSeeTarget(AIPawn, NewLocation.Location))
				{
					OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), NewLocation.Location);
					return;
				}
				SafeCount++;
				if (SafeCount >= 10)	//Perhaps AI is facing a wall, cannot move ahead!
				{
					OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), NewLocation.Location);
					return;
				}
			}
		}
	}
}

bool UBTService_SetRandomLocation::CanSeeTarget(APawn* OwnerPawn, const FVector& TargetLocation)
{
	//Get normalized vector from this current location to target location
	if (!IsValid(OwnerPawn))
		return false;
	FVector DirectionToTarget = (TargetLocation - OwnerPawn->GetActorLocation()).GetSafeNormal(1.0);

	//Get OwnerPawn's viewpoint
	FVector Location;
	FRotator Rotation;
	if (IsValid(OwnerPawn->GetController()))
		OwnerPawn->GetController()->GetPlayerViewPoint(Location, Rotation);

	//Turn OwnerPawn's rotation into normalized vector
	FVector ForwardVector = Rotation.Vector().GetSafeNormal(1.0);

	//Get Dot Product
	float Dot = FVector::DotProduct(ForwardVector, DirectionToTarget);

	//Get angle between this AI's forward vector and the vector to target
	float Angle = UKismetMathLibrary::Acos(Dot) * (180 / PI);

	//If Angle < AngleOfSight, then TargetActor is in this AI's field of vision
	return Angle < AngleOfSight;
}