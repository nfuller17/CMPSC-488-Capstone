// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/NavigationObjectBase.h"
#include "WeaponMaterial.h"
#include "Factory_Material.generated.h"

class AWeaponMaterial;

UCLASS()
class TOMANDJERRY_API AFactory_Material : public ANavigationObjectBase
{
	GENERATED_BODY()
public:
	//Called by player pawn after dying with materials held
	void SpawnMaterial();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<AWeaponMaterial> Material;
	
};
