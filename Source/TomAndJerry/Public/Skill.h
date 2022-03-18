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
	uint8 GetEnergyCost() const{return EnergyCost;}
	virtual bool CanExecute(APawnMonster* Monster);
	virtual void Execute() PURE_VIRTUAL(ASkill::Execute, return;);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere)
	uint8 EnergyCost = 0;
	
private:
	UPROPERTY(EditAnywhere)
	UParticleSystem* Effect;

};
