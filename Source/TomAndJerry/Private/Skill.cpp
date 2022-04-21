// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill.h"
#include "PawnJerry.h"

// Sets default values
ASkill::ASkill()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Effect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Effect"));
	Effect->SetupAttachment(Root);

}

// Called when the game starts or when spawned
void ASkill::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASkill::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	APawnMonster* Monster = Cast<APawnMonster>(GetOwner());
	if (Monster && Monster->IsDead())
	{
		Destroy();
	}

}

bool ASkill::CanExecute(const APawnMonster* Monster) const
{
	if (Monster == nullptr)
		return false;
	return true;
}

void ASkill::Destroyed()
{
	Super::Destroyed();
	if (GetOwner() != nullptr)
	{
		APawnMonster* Monster = Cast<APawnMonster>(GetOwner());
		if (Monster != nullptr)
			Monster->SetSkillIsActive(false);
		else
		{
			APawnJerry* Player = Cast<APawnJerry>(GetOwner());
			if (Player != nullptr)
			{
				Player->SetSkill(nullptr);
			}
		}
	}
}