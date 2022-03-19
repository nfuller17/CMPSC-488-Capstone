// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_UpdateSkillRegenerate.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "PawnMonster.h"
#include "Skill_Regenerate.h"

UBTService_UpdateSkillRegenerate::UBTService_UpdateSkillRegenerate()
{
	NodeName = TEXT("Update Has Regenerate");
	bNotifyBecomeRelevant = true;
}

void UBTService_UpdateSkillRegenerate::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (AIController == nullptr)
		return;
	APawnMonster* Monster = Cast<APawnMonster>(AIController->GetPawn());
	if (Monster == nullptr)
		return;
	if (Monster->HasSkill(ASkill_Regenerate::StaticClass()))
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(GetSelectedBlackboardKey(), true);		
	}
	else
	{
		OwnerComp.GetBlackboardComponent()->ClearValue(GetSelectedBlackboardKey());
	}
}