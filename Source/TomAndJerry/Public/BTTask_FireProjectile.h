// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_FireProjectile.generated.h"

/**
 * 
 */
UCLASS()
class TOMANDJERRY_API UBTTask_FireProjectile : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTTask_FireProjectile();
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
