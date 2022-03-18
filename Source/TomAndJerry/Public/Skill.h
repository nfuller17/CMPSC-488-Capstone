// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PawnMonster.h"
#include "Skill.generated.h"

class APawnMonster;

UCLASS()
class TOMANDJERRY_API ASkill : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASkill();
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual uint8 GetEnergyCost() const{return EnergyCost;}
	virtual bool CanExecute(const APawnMonster* Monster) const;
	virtual void Execute() PURE_VIRTUAL(ASkill::Execute, return;);
	virtual void Destroyed() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	//How much Energy this skill costs
	UPROPERTY(EditAnywhere)
	uint8 EnergyCost = 0;
	//The effect to play on the Pawn that casts this skill
	UPROPERTY(EditAnywhere)
	UParticleSystem* Effect;
	
private:

};
