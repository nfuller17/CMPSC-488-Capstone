// Fill out your copyright notice in the Description page of Project Settings.


#include "ControllerSimple.h"
#include "EngineUtils.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NOB_TwoPoints.h"

void AControllerSimple::BeginPlay()
{
	Super::BeginPlay();
	if (AIBehavior != nullptr)
	{
		RunBehaviorTree(AIBehavior);
		FVector StartLocation;
		FVector EndLocation;
		for (auto NOBLocation : TActorRange<ANOB_TwoPoints>(GetWorld()))
		{
			if (NOBLocation->bStart)
				StartLocation = NOBLocation->GetActorLocation();
			else
				EndLocation = NOBLocation->GetActorLocation();
		}
		GetBlackboardComponent()->SetValueAsVector(TEXT("PointA"), StartLocation);
		GetBlackboardComponent()->SetValueAsVector(TEXT("PointB"), EndLocation);
	}

}