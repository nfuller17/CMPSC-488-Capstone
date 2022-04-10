// Fill out your copyright notice in the Description page of Project Settings.


#include "Singularity.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"
#include "PawnMonster.h"

// Sets default values
ASingularity::ASingularity()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASingularity::BeginPlay()
{
	Super::BeginPlay();
	if (Effect != nullptr)
	{
		UGameplayStatics::SpawnEmitterAttached(Effect, GetRootComponent());
	}
}

// Called every frame
void ASingularity::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	for (APawnMonster* Monster : TActorRange<APawnMonster>(GetWorld()))
	{
		if (!Monster->IsPlayerTeam() && !Monster->IsDead())
		{
			FVector Direction = GetActorLocation() - Monster->GetActorLocation();
			float Distance = Direction.Size();
			if (Distance <= PullRadius)
			{
				FVector Attraction = Direction.GetSafeNormal(1.0) * (PullStrength * FMath::Square((1 - Distance) / PullRadius));
				Monster->LaunchCharacter(Attraction, false, false);
			}
		}
	}
}

void ASingularity::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	UE_LOG(LogTemp, Warning, TEXT("HELLO?"));
	APawnMonster* Monster = Cast<APawnMonster>(OtherActor);
	if (Monster != nullptr && !Monster->IsPlayerTeam() && !Monster->IsDead() && GetOwner() != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Damaging monster from black hole"));
		AController* OwnerController = GetInstigatorController();
		FPointDamageEvent DamageEvent(Damage, FHitResult(), GetActorLocation() - Monster->GetActorLocation(), nullptr);
		Monster->TakeDamage(Damage, DamageEvent, OwnerController, this);
	}
}

