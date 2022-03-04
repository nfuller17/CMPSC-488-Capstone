// Fill out your copyright notice in the Description page of Project Settings.


#include "MinorObjective.h"

// Sets default values
AMinorObjective::AMinorObjective()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMinorObjective::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMinorObjective::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called when player stands on Minor Objective
// Set a Timer that calls a callback function to complete the Minor Objective
void AMinorObjective::BeginActivation()
{
	GetWorldTimerManager().SetTimer(ActivationTimer, this, &AMinorObjective::Complete, CompletionTime);
	UE_LOG(LogTemp, Warning, TEXT("Activating Minor Objective..."));
}
// Called when player walks off Minor Objective
// Reset the timer
void AMinorObjective::StopActivation()
{
	GetWorldTimerManager().ClearTimer(ActivationTimer);
	SetActivatingPawn(nullptr);
	UE_LOG(LogTemp, Warning, TEXT("De-activated Minor Objective"));
}
// Called when player successfully completes Minor Objective after CompletionTime seconds
void AMinorObjective::Complete()
{
	GetWorldTimerManager().ClearTimer(ActivationTimer);
	UE_LOG(LogTemp, Warning, TEXT("Minor Objective completed!"));
}
