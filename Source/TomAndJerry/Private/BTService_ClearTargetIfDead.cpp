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
	if (!IsValid(AIController))
		return;
	if (!IsValid(OwnerComp.GetBlackboardComponent()))
		return;
	AActor* FocusActor = AIController->GetFocusActor();
	if (!IsValid(FocusActor))	//Previous focus actor destroyed
	{
		OwnerComp.GetBlackboardComponent()->ClearValue(GetSelectedBlackboardKey());
	}
	else
	{
		APawnMonster* Monster = Cast<APawnMonster>(FocusActor);
		APawnJerry* Player = Cast<APawnJerry>(FocusActor);
		if (IsValid(Monster) && Monster->IsDead() || IsValid(Player) && Player->IsDead())
		{
			OwnerComp.GetBlackboardComponent()->ClearValue(GetSelectedBlackboardKey());
			AIController->ClearFocus(EAIFocusPriority::Gameplay);
		}
	}
}