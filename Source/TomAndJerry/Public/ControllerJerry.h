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
	virtual void GameHasEnded(class AActor* EndGameFocus = nullptr, bool bIsWinner = false) override;
	UPROPERTY(EditAnywhere)
	float RestartDelay = 5.f;
	UFUNCTION(BlueprintPure)
	UUserWidget* GetHUDWidget() const { return HUD; }
	UFUNCTION(BlueprintCallable)
	void Spectate(const bool& bSpectate);
	FTimerHandle RestartTimer;
	void OpenMenu();
	UFUNCTION(BlueprintCallable)
	void CreateHUD();
	UFUNCTION(BlueprintCallable)
	void DestroyHUD();
	
protected:
	virtual void BeginPlay() override;
	virtual ASpectatorPawn* SpawnSpectatorPawn() override;

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> HUDClass;
	UPROPERTY()
	UUserWidget* HUD;
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> WinScreenClass;
	UPROPERTY()
	UUserWidget* WinScreen;
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> LoseScreenClass;
	UPROPERTY()
	UUserWidget* LoseScreen;
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> InGameMenuClass;
	UPROPERTY()
	UUserWidget* InGameMenu;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class ASpectatorPawn> JerrySpectatorPawn;
};
