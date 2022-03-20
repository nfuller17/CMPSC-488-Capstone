// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AmmoComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TOMANDJERRY_API UAmmoComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAmmoComponent();
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	uint16 GetAmmoAmount() const {return Ammo;}
	void SetAmmoAmount(const uint16& Amount){Ammo = Amount;}
	uint16 GetAmmoMax() const {return AmmoMax;}
	void SetAmmoMax(const uint16& Amount){AmmoMax = Amount;}
	void AddAmmo(const uint16& Amount);
	void ConsumeAmmo(const uint16& Amount);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	//Current ammo count
	uint16 Ammo;
	//How much ammo to start with. All weapons share the same instance of ammo.
	UPROPERTY(EditDefaultsOnly, Category="Ammo", meta=(ClampMin= "0"))
	uint16 StartingAmmo = 50;
	//Max ammo amount for all weapons. Ammo is shared across all weapons.
	UPROPERTY(EditDefaultsOnly, Category="Ammo", meta=(ClampMin= "1"))
	uint16 AmmoMax = 100;
};
