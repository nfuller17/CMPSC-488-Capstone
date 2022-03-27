// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_SetMonsterLocation.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Pawn.h"
#include "AIController.h"

UBTService_SetMonsterLocation::UBTService_SetMonsterLocation()
{
	NodeName = TEXT("Set Monster Actor Key");
}

void UBTService_SetMonsterLocation::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (AIController == nullptr)
		return;
	APawn* AIPawn = AIController->GetPawn();
	if (AIPawn == nullptr)
		return;
	OwnerComp.GetBlackboardComponent()->SetValueAsObject(GetSelectedBlackboardKey(), AIPawn);
}