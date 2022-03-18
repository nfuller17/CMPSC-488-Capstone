// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_UpdateSkillRegenerate.generated.h"

/**
 * 
 */
UCLASS()
class TOMANDJERRY_API UBTService_UpdateSkillRegenerate : public UBTService_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTService_UpdateSkillRegenerate();
protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
