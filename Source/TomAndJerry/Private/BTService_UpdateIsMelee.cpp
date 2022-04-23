// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_UpdateIsMelee.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "PawnMonster.h"

UBTService_UpdateIsMelee::UBTService_UpdateIsMelee()
{
	NodeName = TEXT("Update Is Melee");
}

void UBTService_UpdateIsMelee::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AAIController* AI = OwnerComp.GetAIOwner();
	if (!IsValid(AI))
		return;
	APawnMonster* AIMonster = Cast<APawnMonster>(AI->GetPawn());
	if (!IsValid(AIMonster))
		return;
	if (IsValid(OwnerComp.GetBlackboardComponent()))
	{
		if (AIMonster->IsMelee())
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(GetSelectedBlackboardKey(), true);
		}
		else
		{
			OwnerComp.GetBlackboardComponent()->ClearValue(GetSelectedBlackboardKey());
		}
	}
}