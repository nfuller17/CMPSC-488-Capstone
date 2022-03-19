// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_DoSkill_DamageBoost.generated.h"

/**
 * 
 */
UCLASS()
class TOMANDJERRY_API UBTTask_DoSkill_DamageBoost : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTTask_DoSkill_DamageBoost();
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
