// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_SetRandomLocation.h"
#include "BehaviorTree/BlackboardComponent.h"
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
	if (AI == nullptr)
		return;
	if (AI->GetPawn() == nullptr)
		return;
	FVector CurrentLocation = AI->GetPawn()->GetActorLocation();
	FNavLocation NewLocation = FNavLocation();
	
	//First, see if we can find a valid point nearby us
	//Move there if the AI does not have a line of sight to it (i.e. in a building, behind a wall, etc.)
	UNavigationSystemV1* Navigation = Cast<UNavigationSystemV1>(GetWorld()->GetNavigationSystem());
	if (Navigation == nullptr)
		return;
	if (Navigation->GetRandomReachablePointInRadius(CurrentLocation, CloseDistanceToRoam, NewLocation))
	{
		FHitResult Hit;
		//See if we do *not* have line of sight
		if (!GetWorld()->LineTraceSingleByChannel(Hit, CurrentLocation, NewLocation.Location, ECollisionChannel::ECC_Camera))
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), NewLocation.Location);
		}
		else
		//We do not have line of sight.. find a far location
		{
			bool bValidLocation = false;
			while (!bValidLocation)
			{
				if (Navigation->GetRandomReachablePointInRadius(CurrentLocation, FarDistanceToRoam, NewLocation))
				{
					if ((NewLocation.Location - CurrentLocation).Size() >= FarDistanceToRoam-CloseDistanceToRoam )	//New location must be at least beyond the close distance roam radius value
					{
						OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), NewLocation.Location);
						bValidLocation = true;
					}
				}
			}
		}
	}
	//Otherwise, find a point in a larger radius
}