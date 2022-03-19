// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_DoSkill_Shield.h"
#include "AIController.h"
#include "PawnMonster.h"
#include "Skill_Shield.h"

UBTTask_DoSkill_Shield::UBTTask_DoSkill_Shield()
{
	NodeName = TEXT("Do Skill - Shield");
}

EBTNodeResult::Type UBTTask_DoSkill_Shield::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (AIController == nullptr)
		return EBTNodeResult::Failed;
	APawnMonster* Monster = Cast<APawnMonster>(AIController->GetPawn());
	if (Monster == nullptr)
		return EBTNodeResult::Failed;
	Monster->DoSkill(ASkill_Shield::StaticClass());
	return EBTNodeResult::Succeeded;
}