// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_SelectSkill.generated.h"

/**
 * 
 */
UCLASS()
class TOMANDJERRY_API UBTTask_SelectSkill : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTTask_SelectSkill();
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
