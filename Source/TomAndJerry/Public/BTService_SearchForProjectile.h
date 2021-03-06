// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "Projectile.h"
#include "BTService_SearchForProjectile.generated.h"

/**
 * 
 */
UCLASS()
class TOMANDJERRY_API UBTService_SearchForProjectile : public UBTService_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTService_SearchForProjectile();
	bool CanSeeTarget(APawn* OwnerPawn, AActor* TargetActor);
	bool ProjectileWillCollide(APawn* OwnerPawn, AActor* Proj);
protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
private:
	const float ProjectileDetectionRadius = 3500.0;
	const float ProjectileDodgeRadius = 500.0;	//Dodge projectile if it will be within this radius
	const float AngleOfSight = 100.0;
	const float DodgeStrength = 1000.0;
	const float ZAdd = 400;
};
