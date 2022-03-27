// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_UpdateForwardVector.generated.h"

/**
 * 
 */
UCLASS()
class TOMANDJERRY_API UBTService_UpdateForwardVector : public UBTService_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTService_UpdateForwardVector();
protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
