// Fill out your copyright notice in the Description page of Project Settings.


#include "ControllerTom.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"

//Called when game begins play
//Sets focus of AI to player (AI will constantly look at player)
	//To do later: use SetFocus to focus on an Actor, which can move around - use w/ player
	//Use SetFocalPoint to focus on a location (vector), which is fixed - use w/ materials, minor objectives, weapon craft location
void AControllerTom::BeginPlay()
{
	Super::BeginPlay();
	if(AIBehavior != nullptr){
	    RunBehaviorTree(AIBehavior);
    }
}

//Called every frame
void AControllerTom::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}