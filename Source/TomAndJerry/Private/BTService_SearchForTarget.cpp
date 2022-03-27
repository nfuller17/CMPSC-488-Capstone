// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_SearchForTarget.h"
#include "EngineUtils.h"
#include "Math/UnrealMathUtility.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/Pawn.h"
#include "PawnMonster.h"
#include "PawnJerry.h"
#include "AIController.h"

UBTService_SearchForTarget::UBTService_SearchForTarget()
{
	NodeName = TEXT("Search for Targets");
}

void UBTService_SearchForTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (AIController == nullptr)
		return;
	APawnMonster* PawnOwner = Cast<APawnMonster>(AIController->GetPawn());
	if (PawnOwner == nullptr)
		return;

	//Clear focus for this AI if focus actor is dead, out of range, or if we do not have a line of sight
	AActor* FocusActor = AIController->GetFocusActor();
	if (FocusActor != nullptr)
	{
		if (!AIController->LineOfSightTo(FocusActor))	//No direct line of sight
		{
			AIController->ClearFocus(EAIFocusPriority::Gameplay);
			OwnerComp.GetBlackboardComponent()->ClearValue(GetSelectedBlackboardKey());
		}
		else if ((PawnOwner->GetActorLocation() - FocusActor->GetActorLocation()).Size() > MaxSightDistance)	//Out of distance
		{
			AIController->ClearFocus(EAIFocusPriority::Gameplay);
			OwnerComp.GetBlackboardComponent()->ClearValue(GetSelectedBlackboardKey());
		}
		else
		{
			APawnJerry* Player = Cast<APawnJerry>(FocusActor);
			APawnMonster* Monster = Cast<APawnMonster>(FocusActor);
			if (Monster != nullptr && Monster->IsDead() || Player != nullptr && Player->IsDead())	//Focus actor is dead
			{
				AIController->ClearFocus(EAIFocusPriority::Gameplay);
				OwnerComp.GetBlackboardComponent()->ClearValue(GetSelectedBlackboardKey());
			}
		}
	}

	FocusActor = AIController->GetFocusActor();		//Re-assign if focus was just cleared
	const bool bOnPlayerTeam = PawnOwner->IsPlayerTeam();

	//If this AI is friendly and is following player, search for enemies that have a focus on the player
	//If found, switch our focus from player to enemy
	//Otherwise, keep focus on the player (unless player is out of range or we lose visibility)
	if (FocusActor != nullptr && bOnPlayerTeam && Cast<APawnJerry>(FocusActor) != nullptr)
	{
		for (auto Target : TActorRange<APawnMonster>(PawnOwner->GetWorld()))
		{
			AAIController* TargetController = Cast<AAIController>(Target->GetController());
			if (TargetController != nullptr && !Target->IsPlayerTeam() && !Target->IsDead() && Cast<APawnJerry>(TargetController->GetFocusActor()) != nullptr)
			{
				//Is Target in range of this AI?
				if ((Target->GetActorLocation() - PawnOwner->GetActorLocation()).Size() <= ProtectPlayerFromMonsterRadius)
				{
					AIController->SetFocus(Target);
					OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), Target->GetActorLocation());
					return;
				}
			}
		}
	}

	//Another special case - if this AI is not friendly to player, and if it is currently targeting an AI that is friendly to player
	//If player is nearby, switch focus from the friendly AI to player
	if (FocusActor != nullptr && !bOnPlayerTeam && Cast<APawnMonster>(FocusActor) != nullptr)
	{
		APawn* Player = PawnOwner->GetWorld()->GetFirstPlayerController()->GetPawn();
		if (Player != nullptr)
		{
			//Is Player in range of this AI?
			if ((Player->GetActorLocation() - PawnOwner->GetActorLocation()).Size() <= ChangeFocusToPlayerRadius)
			{
				AIController->SetFocus(Player);
				OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), Player->GetActorLocation());
				return;
			}
		}
	}

	//Already have a target, let's not search for a new one just yet
	if (FocusActor != nullptr)
		return;

	float ClosestDistance = MaxSightDistance;
	APawn* SelectedTarget = nullptr;


	//Check to see if an enemy is within our "eye sight"
	//Loop through all Pawns that this AI can potentially see
	for (auto Target : TActorRange<APawn>(PawnOwner->GetWorld()))
	{
		//Simple intial check if we have a direct line of sight to Target
		if (!AIController->LineOfSightTo(Target))
			continue;

		APawnMonster* TargetMonster = Cast<APawnMonster>(Target);
		APawnJerry* Player = Cast<APawnJerry>(Target);
		float Distance = 0.0;

		if ((bOnPlayerTeam && TargetMonster != nullptr && !TargetMonster->IsPlayerTeam())		//On player team - check for Bosses or Minions
			|| (!bOnPlayerTeam && TargetMonster != nullptr && TargetMonster->IsPlayerTeam())	//On Monster team - check for Players or Allies
			|| Player != nullptr)	//Can always target player, whether friendly or hostile AI. In case of friendly AI, "targeting" player simply means follow player	
		{
			if (bOnPlayerTeam && Player != nullptr && Cast<APawnMonster>(AIController->GetFocusActor()) != nullptr)		//Friendly AI is currently fighting a monster. Do not start following player
				continue;
			if (CanSeeTarget(PawnOwner, Target, Distance))
			{
				if (Distance < ClosestDistance)	//This Target's distance is closer than another's, choose this as our Selected Target
				{
					SelectedTarget = Target;
					ClosestDistance = Distance;

				}
			}
		}
	}

	if (SelectedTarget != nullptr) 		//Found a target for this AI
	{
		UE_LOG(LogTemp, Warning, TEXT("FOCUS SET!"));
		AIController->SetFocus(SelectedTarget);
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), SelectedTarget->GetActorLocation());
	}
	else
	{
		//No target in sight. Clear focus and clear blackboard key
		AIController->ClearFocus(EAIFocusPriority::Gameplay);
		OwnerComp.GetBlackboardComponent()->ClearValue(GetSelectedBlackboardKey());
	}
}

bool UBTService_SearchForTarget::CanSeeTarget(APawn* OwnerPawn, APawn* TargetPawn, float& Distance)
{
	//Get vector from this AI to target
	FVector DirectionToTarget = (TargetPawn->GetActorLocation() - OwnerPawn->GetActorLocation());
	Distance = DirectionToTarget.Size();

	//Is Target within range?
	if (Distance > MaxSightDistance)
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
	UE_LOG(LogTemp, Warning, TEXT("Size of Direction is: %f"), DirectionToTarget.Size());

	//Get Dot Product
	float Dot = FVector::DotProduct(ForwardVector, DirectionToTarget);
	UE_LOG(LogTemp, Warning, TEXT("Dot is: %f"), Dot);

	//Get radians between this AI's forward vector and the vector to target
	float Radians = UKismetMathLibrary::Acos(Dot);
	float Angle = Radians * (180 / PI);

	//If Angle < AngleOfSight, then TargetPawn is in this AI's field of vision
	return Angle < AngleOfSight;
}