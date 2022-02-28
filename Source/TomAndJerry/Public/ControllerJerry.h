// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ControllerJerry.generated.h"

//Forward class declaration - ie tell Compiler to compile this class first
class AWeaponMaterial;

UCLASS()
class TOMANDJERRY_API AControllerJerry : public APlayerController
{
	GENERATED_BODY()

public:
	void AddWeaponMaterial(uint8 WeaponMaterialNumber);
	TArray<uint8> GetMaterials() { return MaterialInventory; }
	void CollectMaterials();
	virtual void GameHasEnded(class AActor* EndGameFocus = nullptr, bool bIsWinner = false) override;
	UPROPERTY(EditAnywhere)
	float RestartDelay = 5.f;
	FTimerHandle RestartTimer;

private:
	TArray<uint8>	MaterialInventory;
	UPROPERTY(EditAnywhere)
	float MaxSpeed = 400.00;
};
