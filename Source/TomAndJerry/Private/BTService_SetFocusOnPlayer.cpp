// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_SetFocusOnPlayer.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "AIController.h"

UBTService_SetFocusOnPlayer::UBTService_SetFocusOnPlayer()
{
	NodeName = TEXT("Set Focus on Player");
}

void UBTService_SetFocusOnPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);			//Won't work exactly with multiple players, can maybe try for closest player
	if (PlayerPawn == nullptr)
		return;
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (AIController == nullptr)
		return;
	AIController->SetFocus(PlayerPawn);
}