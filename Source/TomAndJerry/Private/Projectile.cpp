// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
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
	if (IsValid(Mesh))
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
	
	if (IsValid(Root))
	{
		Root->MoveComponent(Speed*DeltaTime*(GetActorRotation().Vector()), GetActorRotation(), true);
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
	AController* OwnerController = GetInstigatorController();
	if (OwnerController == nullptr)
		return;
	if (bSplashDamage)	//Damage any Pawns within our radius
	{
		TArray< TEnumAsByte< EObjectTypeQuery > > ObjectTypes;
		ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));
		ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel4));
		TArray < AActor*> IgnoreActors;
		TArray< AActor* > FoundActors;
		if (UKismetSystemLibrary::SphereOverlapActors(GetWorld(), GetActorLocation(), DamageRadius, ObjectTypes, APawn::StaticClass(), IgnoreActors, FoundActors))
		{
			for (auto Victim : FoundActors)
			{
				ServeDamage(Victim, OwnerController);
			}
		}
	}
	else	//Must land direct hit
	{
		if (TargetActor != nullptr)
		{
			//Damage the TargetActor
			ServeDamage(TargetActor, OwnerController);			
		}
	}
	Destroy();
}

void AProjectile::ServeDamage(AActor* Victim, AController* OwnerController)
{
	if (!IsValid(Victim))	//Safety check
		return;
	APawnBoss* Boss = Cast<APawnBoss>(Victim);
	if (IsValid(Boss) && !CanDamageBoss)
		return;
	FPointDamageEvent DamageEvent(Damage, FHitResult(), GetActorLocation() - Victim->GetActorLocation(), nullptr);
	Victim->TakeDamage(Damage, DamageEvent, OwnerController, this);
}

