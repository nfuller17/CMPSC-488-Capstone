// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_MeleeAttack.h"
#include "AIController.h"
#include "PawnMonster.h"

UBTTask_MeleeAttack::UBTTask_MeleeAttack()
{
	NodeName = TEXT("Melee Attack");
}

EBTNodeResult::Type UBTTask_MeleeAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!IsValid(AIController))
		return EBTNodeResult::Failed;
	APawnMonster* Monster = Cast<APawnMonster>(AIController->GetPawn());
	if (!IsValid(Monster))
		return EBTNodeResult::Failed;
	AActor* Target = AIController->GetFocusActor();
	if (Target == nullptr)
		return EBTNodeResult::Failed;
	Monster->StartMelee(Target);
	return EBTNodeResult::Succeeded;
}