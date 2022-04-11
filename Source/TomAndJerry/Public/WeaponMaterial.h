// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Factory_Material.h"
#include "WeaponMaterial.generated.h"

class AFactory_Material;

UCLASS()
class TOMANDJERRY_API AWeaponMaterial : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponMaterial();
	void SetMaterialID(uint8 MaterialId);
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	// Get Material ID. Rather than assigning the ID in C++, we will make the variable accessible in Blueprint, and assign it there
	uint8 GetMaterialID() { return MaterialID; }
	void SetFactory(AFactory_Material* _Factory) { Factory = _Factory; }
	AFactory_Material* GetFactory() const { return Factory; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly)
	uint8 MaterialID;
	AFactory_Material* Factory;
};
