// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_UpdateTargetLocation.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

UBTService_UpdateTargetLocation::UBTService_UpdateTargetLocation()
{
	NodeName = TEXT("Update Target Location");
}

void UBTService_UpdateTargetLocation::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AAIController* AIController = OwnerComp.GetAIOwner();
	if (AIController == nullptr)
		return;
	AActor* FocusActor = AIController->GetFocusActor();
	if (FocusActor == nullptr)
		return;
	OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), FocusActor->GetActorLocation());
}