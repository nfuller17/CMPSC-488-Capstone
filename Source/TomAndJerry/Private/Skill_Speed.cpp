// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill_Speed.h"

#include "GameFramework/CharacterMovementComponent.h"

void ASkill_Speed::BeginPlay()
{
	Super::BeginPlay();
	SetLifeSpan(Duration);
}

void ASkill_Speed::Execute()
{
	//Do not call Super!
	ACharacter* Pawn = Cast<ACharacter>(GetOwner());
	if (Pawn == nullptr)
		return;
	UCharacterMovementComponent* Movement = Pawn->GetCharacterMovement();
	if (Movement == nullptr)
		return;
	Movement->MaxWalkSpeed = Movement->MaxWalkSpeed * SpeedMultiplier;
}

void ASkill_Speed::Destroyed()
{
	Super::Destroyed();
	ACharacter* Pawn = Cast<ACharacter>(GetOwner());
	if (Pawn != nullptr)
	{
		UCharacterMovementComponent* Movement = Pawn->GetCharacterMovement();
		if (Movement != nullptr)
		{
			Movement->MaxWalkSpeed = Movement->MaxWalkSpeed * (1 / SpeedMultiplier);
		}
	}

}