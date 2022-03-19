// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_UpdateSkillDamageBoost.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "PawnMonster.h"
#include "Skill_DamageBoost.h"

UBTService_UpdateSkillDamageBoost::UBTService_UpdateSkillDamageBoost()
{
	NodeName = TEXT("Update Has Damage Boost");
}

void UBTService_UpdateSkillDamageBoost::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (AIController == nullptr)
		return;
	APawnMonster* Monster = Cast<APawnMonster>(AIController->GetPawn());
	if (Monster == nullptr)
		return;
	if (Monster->HasSkill(ASkill_DamageBoost::StaticClass()))
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(GetSelectedBlackboardKey(), true);		
	}
	else
	{
		OwnerComp.GetBlackboardComponent()->ClearValue(GetSelectedBlackboardKey());
	}
	
}