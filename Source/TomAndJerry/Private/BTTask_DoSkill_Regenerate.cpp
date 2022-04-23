// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_DoSkill_Regenerate.h"
#include "AIController.h"
#include "PawnMonster.h"
#include "Skill_Regenerate.h"

UBTTask_DoSkill_Regenerate::UBTTask_DoSkill_Regenerate()
{
	NodeName = TEXT("Do Skill - Regenerate");	
}

EBTNodeResult::Type UBTTask_DoSkill_Regenerate::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!IsValid(AIController))
		return EBTNodeResult::Failed;
	APawnMonster* Monster = Cast<APawnMonster>(AIController->GetPawn());
	if (!IsValid(Monster))
		return EBTNodeResult::Failed;
	Monster->DoSkill(ASkill_Regenerate::StaticClass());
	return EBTNodeResult::Succeeded;
}