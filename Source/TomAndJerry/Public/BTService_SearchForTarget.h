// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_SearchForTarget.generated.h"

/**
 * 
 */
UCLASS()
class TOMANDJERRY_API UBTService_SearchForTarget : public UBTService_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UBTService_SearchForTarget();
protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	bool CanSeeTarget(APawn* OwnerPawn, APawn* TargetPawn, float& Distance);
private:
	//Angle from Forward vector for field of vision for AI
	const float AngleOfSight = 55.0;
	const float MaxSightDistance = 7000.0;
	const float ProtectPlayerFromMonsterRadius = 3000.0;
};
