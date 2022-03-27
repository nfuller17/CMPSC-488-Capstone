// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_SearchForTargets.generated.h"

/**
 * 
 */
UCLASS()
class TOMANDJERRY_API UBTTask_SearchForTargets : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTTask_SearchForTargets();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	bool CanSeeTarget(APawn* OwnerPawn, APawn* TargetPawn, float& Distance);

private:
	//Angle from Forward vector for field of vision for AI
	const float AngleOfSight = 55.0;
	const float MaxSightDistance = 7000.0;
};
