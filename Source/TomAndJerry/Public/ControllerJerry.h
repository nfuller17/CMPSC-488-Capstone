// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpectatorPawn.h"
#include "ControllerJerry.generated.h"

//Forward class declaration - ie tell Compiler to compile this class first
class AWeaponMaterial;
class ASpectatorPawn;

UCLASS()
class TOMANDJERRY_API AControllerJerry : public APlayerController
{
	GENERATED_BODY()

public:
	void AddWeaponMaterial(uint8 WeaponMaterialNumber);
	TArray<uint8> GetMaterials() { return MaterialInventory; }
	UFUNCTION(BlueprintPure)
		uint8 GetNumMaterialsInv() { return MaterialInventory.Num(); }
	void CollectMaterials();
	virtual void GameHasEnded(class AActor* EndGameFocus = nullptr, bool bIsWinner = false) override;
	UPROPERTY(EditAnywhere)
	float RestartDelay = 5.f;
	UFUNCTION(BlueprintCallable)
	void Spectate(const bool& bSpectate);
	FTimerHandle RestartTimer;
	
protected:
	virtual void BeginPlay() override;
	virtual ASpectatorPawn* SpawnSpectatorPawn() override;

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> HUDClass;
	UPROPERTY()
	UUserWidget* HUD;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class ASpectatorPawn> JerrySpectatorPawn;
	TArray<uint8>	MaterialInventory;
};
