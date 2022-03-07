// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	if (Effect)
	{
		UGameplayStatics::SpawnEmitterAttached(Effect, Root);
	}
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (Root)
	{
		Root->MoveComponent(Speed*DeltaTime*(GetActorRotation().Vector()), GetActorRotation(), false);
	}
}

void AProjectile::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	
	if (OtherActor)
	{
		APawn* OtherPawn = Cast<APawn>(OtherActor);
		if (OtherPawn)
		{
			if (OtherPawn == Cast<APawn>(GetOwner()))
			{
				return;
			}
			Explode();
		}
	}
}

void AProjectile::Explode()
{
	UE_LOG(LogTemp, Warning, TEXT("EXPLODE!"));
	Destroy();
}

