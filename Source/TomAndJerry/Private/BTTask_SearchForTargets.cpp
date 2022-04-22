// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_SearchForTargets.h"
#include "EngineUtils.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Pawn.h"
#include "PawnMonster.h"
#include "PawnJerry.h"
#include "AIController.h"

UBTTask_SearchForTargets::UBTTask_SearchForTargets()
{
	NodeName = TEXT("DEPRECATED - Search for Targets");
}