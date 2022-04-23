// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FireProjectile.h"
#include "AIController.h"
#include "PawnMonster.h"

UBTTask_FireProjectile::UBTTask_FireProjectile()
{
	NodeName = TEXT("Fire Projectile");
}

EBTNodeResult::Type UBTTask_FireProjectile::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!IsValid(AIController))
		return EBTNodeResult::Failed;
	APawnMonster* Monster = Cast<APawnMonster>(AIController->GetPawn());
	if (!IsValid(Monster))
		return EBTNodeResult::Failed;
	Monster->StartFire();
	return EBTNodeResult::Succeeded;
}