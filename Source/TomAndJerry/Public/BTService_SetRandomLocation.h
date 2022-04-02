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
	bool CanSeeTarget(APawn* OwnerPawn, const FVector& TargetLocation);

private:
	float DistanceToRoam = 7000.f;
	float MinimumDistance = 3000.f;
	float AngleOfSight = 60.f;
};
