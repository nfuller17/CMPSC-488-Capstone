// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_CheckTargetIsHostile.h"
#include "PawnMonster.h"
#include "PawnJerry.h"
#include "AIController.h"

UBTTask_CheckTargetIsHostile::UBTTask_CheckTargetIsHostile()
{
	NodeName = TEXT("Check Target is Hostile");
}

//Whether this AI's focus pawn is on the other team
EBTNodeResult::Type UBTTask_CheckTargetIsHostile::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	AAIController* AIController = OwnerComp.GetAIOwner();
	if (AIController == nullptr)
		return EBTNodeResult::Failed;
	AActor* FocusActor = AIController->GetFocusActor();
	if (FocusActor == nullptr)
		return EBTNodeResult::Failed;
	APawnMonster* PawnOwner = Cast<APawnMonster>(AIController->GetPawn());
	if (PawnOwner == nullptr)
		return EBTNodeResult::Failed;

	bool bOnPlayerTeam = PawnOwner->IsPlayerTeam();
	APawnMonster* Monster = Cast<APawnMonster>(FocusActor);
	APawnJerry* Player = Cast<APawnJerry>(FocusActor);
	if (bOnPlayerTeam && Monster != nullptr && !Monster->IsPlayerTeam()
		|| !bOnPlayerTeam && ( Player != nullptr || Monster != nullptr && Monster->IsPlayerTeam() ) )
	{
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}