// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_SetRandomLocation.generated.h"

/**
 * 
 */
UCLASS()
class TOMANDJERRY_API UBTService_SetRandomLocation : public UBTService_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTService_SetRandomLocation();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
private:
	float CloseDistanceToRoam = 3000.f;	//Check for a random location within this radius, valid if we do *not* have a line of sight to this point
	float FarDistanceToRoam = 6000.f;	//Otherwise, check for a random location within this larger radius
};
