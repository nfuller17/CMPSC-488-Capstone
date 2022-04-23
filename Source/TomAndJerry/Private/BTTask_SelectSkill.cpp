// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_SelectSkill.h"

UBTTask_SelectSkill::UBTTask_SelectSkill()
{
	NodeName = TEXT("DEPRECATED - Select Skill");
}

EBTNodeResult::Type UBTTask_SelectSkill::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	return EBTNodeResult::Succeeded;
}