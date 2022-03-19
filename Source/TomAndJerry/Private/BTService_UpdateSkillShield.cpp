// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_UpdateSkillShield.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "PawnMonster.h"
#include "Skill_Shield.h"

UBTService_UpdateSkillShield::UBTService_UpdateSkillShield()
{
	NodeName = TEXT("Update Has Shield");
}

void UBTService_UpdateSkillShield::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (AIController == nullptr)
		return;
	APawnMonster* Monster = Cast<APawnMonster>(AIController->GetPawn());
	if (Monster == nullptr)
		return;
	if (Monster->HasSkill(ASkill_Shield::StaticClass()))
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(GetSelectedBlackboardKey(), true);		
	}
	else
	{
		OwnerComp.GetBlackboardComponent()->ClearValue(GetSelectedBlackboardKey());
	}
}