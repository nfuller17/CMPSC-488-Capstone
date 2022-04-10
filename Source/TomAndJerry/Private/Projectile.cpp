// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "Kismet/GameplayStatics.h"
#include "PawnMonster.h"
#include "PawnJerry.h"
#include "PawnBoss.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Root = CreateDefaultSubobject<USphereComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
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

void AProjectile::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);
	Explode(Other);
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
			Explode(OtherPawn);
		}
	}
}


void AProjectile::Explode(AActor* TargetActor)
{
	if (bSplashDamage)	//Damage any Pawns within our radius
	{
		
	}
	else	//Must land direct hit
	{
		if (TargetActor)
		{
			//Damage the TargetActor
			AController* OwnerController = GetInstigatorController();
			if (OwnerController)
			{
				APawnBoss* Boss = Cast<APawnBoss>(TargetActor);
				if (Boss == nullptr || Boss != nullptr && CanDamageBoss)
				{
					FPointDamageEvent DamageEvent(Damage, FHitResult(), GetActorLocation() - TargetActor->GetActorLocation(), nullptr);
					TargetActor->TakeDamage(Damage, DamageEvent, OwnerController, this);
				}
			}				
		}
	}
	Destroy();
}

