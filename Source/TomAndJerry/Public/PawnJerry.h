#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ControllerJerry.h"
#include "AmmoComponent.h"
#include "PawnJerry.generated.h"

//Forward class declaration - ie tell Compiler to compile this class first
class AWeapon;

UCLASS()
class TOMANDJERRY_API APawnJerry : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APawnJerry();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//Called when Pawn walks over another Actor with collision set to overlap Pawns- in this case, Materials, Minor Objectives, etc.
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	//Called when Player walks away from a previously overlapping Actor- used to determine when Player steps off a Minor Objective
	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;
	
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	
	void AddWeapon(TSubclassOf<AWeapon> WeaponClass);
	
	void AddWeaponMaterial(uint8 WeaponMaterialNumber, FVector WeaponLocation, UStaticMesh* SMesh);
	TArray<uint8> GetMaterials() { return MaterialInventory; }
	TArray<FVector> GetMaterialLocs() { return MaterialLocs; }
	UFUNCTION(BlueprintPure)
		uint8 GetNumMaterialsInv() { return MaterialInventory.Num(); }
	void CollectMaterials();

	UFUNCTION(BlueprintPure)
	bool IsDead() const;
	float GetHealth() const {return Health;}
	UFUNCTION(BlueprintPure)
	int GetUIHealth() const {return FMath::RoundFromZero(Health);}
	float GetHealthMax() const {return HealthMax;}
	void AddHealth(const float& HealthAdd);
	UFUNCTION(BlueprintPure)
	UAmmoComponent* GetAmmoComponent() const {return AmmoComponent;}
	UFUNCTION(BlueprintPure)
	bool HasWeapon(const int& WeaponNum) const;
	FTimerHandle DestroyTimer;
	UFUNCTION(BlueprintCallable)
	void DestroyOnSpectate(const bool& SpectateMode);
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(EditDefaultsOnly)
	float HealthMax = 500;
	UPROPERTY(VisibleAnywhere)
	float Health;
	virtual void Died();
	virtual void DestroyHelper();
	virtual void Destroyed() override;

private:
	
	//Pointer to currently held weapon, if any
	AWeapon* Weapon;
	
	//WeaponInventory is an array of Subclasses of type Weapon
	//It represents the weapons the player has collected
	TArray<TSubclassOf<AWeapon>> WeaponInventory;
	
	//Allows the character to move forward(AxisValue = 1.0) or backward(AxisValue = -1.0)
	//Already framerate independent
	void MoveForward(float AxisValue);
	//Allows the character to move rightward(AxisValue = 1.0) or leftward(AxisValue = -1.0)
	//Already framerate independent
	void Strafe(float AxisValue);
	void Dodge(const int32 Direction);
	DECLARE_DELEGATE_OneParam(FDodgeDelegate, const int32);
	//How quickly in seconds player must successively press a movement key twice to dodge in that direction
	UPROPERTY(EditAnywhere)
	float DodgeTime = 0.35;
	//If after a successful dodge, how long in seconds until player can do another dodge
	UPROPERTY(EditAnywhere)
	float TimeBetweenDodges = 2.0;
	//Time required between two key presses to constitute the input as a double key press
	float DoublePressTime = 0.2;
	//Magnitude of dodge direction
	UPROPERTY(EditAnywhere)
	float DodgeStrength = 1000.0;
	//How much Z-Axis boost when dodging
	UPROPERTY(EditAnywhere)
	float ZAdd = 400;
	float LastDodgeTime = 0.f;
	float LastDirInputTime = 0.f;
	int32 prevDir = 0;
	
	TArray<uint8>	MaterialInventory;
	TArray<FVector>	MaterialLocs;
	TArray<UStaticMesh*> SMeshes;
	
	//Weapon
	void BeginFire();
	void StopFire();
	void SelectWeapon(const int32 WeaponNumber);
	UAmmoComponent* AmmoComponent;
	DECLARE_DELEGATE_OneParam(FSelectWeaponDelegate, const int32);
};

