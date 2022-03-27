// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_ClearTargetIfDead.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "PawnMonster.h"
#include "PawnJerry.h"
#include "AIController.h"

UBTService_ClearTargetIfDead::UBTService_ClearTargetIfDead()
{
	NodeName = TEXT("Clear Target If Dead");
}

void UBTService_ClearTargetIfDead::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (AIController == nullptr)
		return;
	AActor* FocusActor = AIController->GetFocusActor();
	if (FocusActor == nullptr)	//Previous focus actor destroyed
	{
		UE_LOG(LogTemp, Warning, TEXT("Focus Actor is null"));
		OwnerComp.GetBlackboardComponent()->ClearValue(GetSelectedBlackboardKey());
	}
	else
	{
		APawnMonster* Monster = Cast<APawnMonster>(FocusActor);
		APawnJerry* Player = Cast<APawnJerry>(FocusActor);
		if (Monster != nullptr && Monster->IsDead() || Player != nullptr && Player->IsDead())
		{
			UE_LOG(LogTemp, Warning, TEXT("Target is Dead"));
			OwnerComp.GetBlackboardComponent()->ClearValue(GetSelectedBlackboardKey());
			AIController->ClearFocus(EAIFocusPriority::Gameplay);
		}
	}
}