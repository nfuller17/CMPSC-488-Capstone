// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_UpdateIsMelee.generated.h"

/**
 * 
 */
UCLASS()
class TOMANDJERRY_API UBTService_UpdateIsMelee : public UBTService_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTService_UpdateIsMelee();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
};
