// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_CheckTargetIsHostile.generated.h"

/**
 * 
 */
UCLASS()
class TOMANDJERRY_API UBTTask_CheckTargetIsHostile : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTTask_CheckTargetIsHostile();
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
