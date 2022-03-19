// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_DoSkill_DamageBoost.h"
#include "AIController.h"
#include "PawnMonster.h"
#include "Skill_DamageBoost.h"

UBTTask_DoSkill_DamageBoost::UBTTask_DoSkill_DamageBoost()
{
	NodeName = TEXT("Do Skill - Damage Boost");
}

EBTNodeResult::Type UBTTask_DoSkill_DamageBoost::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (AIController == nullptr)
		return EBTNodeResult::Failed;
	APawnMonster* Monster = Cast<APawnMonster>(AIController->GetPawn());
	if (Monster == nullptr)
		return EBTNodeResult::Failed;
	Monster->DoSkill(ASkill_DamageBoost::StaticClass());
	return EBTNodeResult::Succeeded;
}