// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_UpdateForwardVector.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Pawn.h"
#include "AIController.h"

UBTService_UpdateForwardVector::UBTService_UpdateForwardVector()
{
	NodeName = TEXT("Update Forward Vector");
}

void UBTService_UpdateForwardVector::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!IsValid(AIController))
		return;
	APawn* AIPawn = AIController->GetPawn();
	if (!IsValid(AIPawn))
		return;
	if (IsValid(OwnerComp.GetBlackboardComponent()))
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), AIPawn->GetActorForwardVector());
}