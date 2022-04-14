// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Projectile.h"
#include "BTTask_Dodge.generated.h"

/**
 * 
 */
UCLASS()
class TOMANDJERRY_API UBTTask_Dodge : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTTask_Dodge();
	bool CanSeeTarget(APawn* OwnerPawn, AActor* TargetActor);
	bool ProjectileWillCollide(APawn* OwnerPawn, AActor* Proj);
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
private:
	const float ProjectileDetectionRadius = 3500.0;
	const float ProjectileDodgeRadius = 800.0;	//Dodge projectile if it will be within this radius
	const float AngleOfSight = 100.0;
	const float DodgeStrength = 1000.0;
	const float ZAdd = 400;
};
