// Fill out your copyright notice in the Description page of Project Settings.


#include "CraftingPost.h"
#include "PawnJerry.h"
#include "ControllerJerry.h"
#include "../TomAndJerryGameModeBase.h"

// Sets default values
ACraftingPost::ACraftingPost()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACraftingPost::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACraftingPost::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACraftingPost::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	APawnJerry* Jerry = Cast<APawnJerry>(OtherActor);
	if (Jerry)
	{
		// AControllerJerry* JerryController = Cast<AControllerJerry>(Jerry->GetController());
		TArray<uint8> Materials = Jerry->GetMaterials();
		const uint8 NumMaterials = Materials.Num();
		Jerry->CollectMaterials();
		ATomAndJerryGameModeBase* TomAndJerryGame = Cast<ATomAndJerryGameModeBase>(GetWorld()->GetAuthGameMode());
		if (TomAndJerryGame)
		{
			TomAndJerryGame->DepositMaterial(NumMaterials, Jerry);
		}
		UE_LOG(LogTemp, Warning, TEXT("Collected %d materials from player."), NumMaterials);
	}
}

