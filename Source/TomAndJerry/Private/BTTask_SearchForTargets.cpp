// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_SearchForTargets.h"
#include "EngineUtils.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Pawn.h"
#include "PawnMonster.h"
#include "PawnJerry.h"
#include "AIController.h"

UBTTask_SearchForTargets::UBTTask_SearchForTargets()
{
	NodeName = TEXT("DEPRECATED - Search for Targets");
}

EBTNodeResult::Type UBTTask_SearchForTargets::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (AIController == nullptr)
		return  EBTNodeResult::Failed;
	APawnMonster* Monster = Cast<APawnMonster>(AIController->GetPawn());
	if (Monster == nullptr)
		return  EBTNodeResult::Failed;

	const bool bOnPlayerTeam = Monster->IsPlayerTeam();
	float ClosestDistance = MaxSightDistance;
	APawn* SelectedTarget = nullptr;


	//Check to see if an enemy is within our "eye sight"
	//Loop through all Pawns that this AI can potentially see
	for (auto Target : TActorRange<APawn>(Monster->GetWorld()))
	{
		//Simple intial check if we have a direct line of sight to Target
		if (!AIController->LineOfSightTo(Target))
			continue;

		APawnMonster* TargetMonster = Cast<APawnMonster>(Target);
		APawnJerry* Player = Cast<APawnJerry>(Target);
		float Distance = 0.0;

		if ( (bOnPlayerTeam && TargetMonster != nullptr && !TargetMonster->IsPlayerTeam() )		//On player team - check for Bosses or Minions
			|| (!bOnPlayerTeam && TargetMonster != nullptr && TargetMonster->IsPlayerTeam() )	//On Monster team - check for Players or Allies
			|| Player != nullptr)	//Can always target player, whether friendly or hostile AI. In case of friendly AI, "targeting" player simply means follow player	
		{
			if (CanSeeTarget(Monster, Target, Distance))
			{
				if (Distance < ClosestDistance)	//This Target's distance is closer than another's, choose this as our Selected Target
				{
					SelectedTarget = Target;
					ClosestDistance = Distance;

				}
			}
		}
	}

	//Add code to handle when this AI is friendly, and we are following player despite having hostile monsters nearby because player is closest

	if (SelectedTarget != nullptr)
	{
		//Found a target for this AI
		AIController->ClearFocus(EAIFocusPriority::Gameplay);
		AIController->SetFocus(SelectedTarget);
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), SelectedTarget->GetActorLocation());

		//Likely, this target will be hostile. However, if this AI is an ally, and the target is a player, then the target is friendly
		//Return success if target is hostile - behavior tree will move to attack branch.
		//Return fail if target is friendly - behavior tree will move to follow player.
		APawnJerry* Player = Cast<APawnJerry>(SelectedTarget);
		bool bTargetIsFriendly = bOnPlayerTeam && Player != nullptr;
		if (!bTargetIsFriendly)
		{
			UE_LOG(LogTemp, Warning, TEXT("Selected Hostile Target!"));
			return EBTNodeResult::Succeeded;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Selected Friendly Target!"));
			return EBTNodeResult::Failed;
		}
	}
	else
	{
		//No target in sight. Clear focus and clear blackboard key
		AIController->ClearFocus(EAIFocusPriority::Gameplay);
		OwnerComp.GetBlackboardComponent()->ClearValue(GetSelectedBlackboardKey());
		return EBTNodeResult::Failed;	//Automatically switch to Follow Player branch (which will fail)
	}
}

bool UBTTask_SearchForTargets::CanSeeTarget(APawn* OwnerPawn, APawn* TargetPawn, float& Distance)
{
	//Get vector from this AI to target
	FVector DirectionToTarget = (TargetPawn->GetActorLocation() - OwnerPawn->GetActorLocation());
	Distance = DirectionToTarget.Size();

	//Is Target within range?
	if (Distance > MaxSightDistance)
		return false;	//Target too far away

	//Normalize the vector
	DirectionToTarget = DirectionToTarget.GetSafeNormal(1.0);

	//Get angle between this AI's forward vector and the vector to target
	float Angle = FVector::DotProduct(OwnerPawn->GetActorForwardVector(), DirectionToTarget);

	//If Angle < AngleOfSight, then TargetPawn is in this AI's field of vision
	return Angle < AngleOfSight;
}