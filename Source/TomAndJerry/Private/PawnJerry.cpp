// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnJerry.h"
#include "WeaponMaterial.h"
#include "ControllerJerry.h"
#include "MinorObjective.h"
#include "Weapon.h"
#include "Components/CapsuleComponent.h"
#include "PawnMonster.h"
#include "GameFramework/GameModeBase.h"
#include "../TomAndJerryGameModeBase.h"
#include "Skill_Amplify.h"

// Sets default values
APawnJerry::APawnJerry()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void APawnJerry::BeginPlay()
{
	Super::BeginPlay();
	
	//Hide the weapon mesh that comes with the Wraith mesh
	if (IsValid(GetMesh()))
		GetMesh()->HideBoneByName(TEXT("weapon_r"), EPhysBodyOp::PBO_None);
	AmmoComponent = FindComponentByClass<UAmmoComponent>();
	Health = HealthMax;

	//Start energy regen timer
	GetWorldTimerManager().SetTimer(EnergyTimer, this, &APawnJerry::AddEnergyHelper, EnergyRegenRate, true, 0.0);

	//If we respawned after dying and we had a super weapon, re-create our super weapon
	ATomAndJerryGameModeBase* Game = Cast<ATomAndJerryGameModeBase>(GetWorld()->GetAuthGameMode());
	if (IsValid(Game) && Game->HasSuperWeapon())
	{
		Game->SpawnSuperWeapon(this);
	}
}

// Called every frame
void APawnJerry::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APawnJerry::DestroyOnSpectate(const bool& SpectateMode)
{
	if (SpectateMode)
		Destroy();
}

// Called to bind functionality to input
void APawnJerry::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (IsValid(PlayerInputComponent))
	{
		PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &APawnJerry::MoveForward);
		PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &APawn::AddControllerPitchInput);
		PlayerInputComponent->BindAxis(TEXT("Strafe"), this, &APawnJerry::Strafe);
		PlayerInputComponent->BindAxis(TEXT("LookRight"), this, &APawn::AddControllerYawInput);
		PlayerInputComponent->BindAction<FSelectWeaponDelegate>(TEXT("SelectWeapon1"), EInputEvent::IE_Pressed, this, &APawnJerry::SelectWeapon, 1);
		PlayerInputComponent->BindAction<FSelectWeaponDelegate>(TEXT("SelectWeapon2"), EInputEvent::IE_Pressed, this, &APawnJerry::SelectWeapon, 2);
		PlayerInputComponent->BindAction<FSelectWeaponDelegate>(TEXT("SelectWeapon3"), EInputEvent::IE_Pressed, this, &APawnJerry::SelectWeapon, 3);
		PlayerInputComponent->BindAction<FSelectWeaponDelegate>(TEXT("SelectWeapon4"), EInputEvent::IE_Pressed, this, &APawnJerry::SelectWeapon, 4);
		PlayerInputComponent->BindAction<FSelectWeaponDelegate>(TEXT("SelectWeapon5"), EInputEvent::IE_Pressed, this, &APawnJerry::SelectWeapon, 5);
		PlayerInputComponent->BindAction<FSelectWeaponDelegate>(TEXT("SelectWeapon6"), EInputEvent::IE_Pressed, this, &APawnJerry::SelectWeapon, 6);
		PlayerInputComponent->BindAction<FSelectWeaponDelegate>(TEXT("SelectWeapon7"), EInputEvent::IE_Pressed, this, &APawnJerry::SelectWeapon, 7);
		PlayerInputComponent->BindAction<FSelectWeaponDelegate>(TEXT("SelectWeapon8"), EInputEvent::IE_Pressed, this, &APawnJerry::SelectWeapon, 8);
		PlayerInputComponent->BindAction<FSelectWeaponDelegate>(TEXT("SelectWeapon9"), EInputEvent::IE_Pressed, this, &APawnJerry::SelectWeapon, 9);
		PlayerInputComponent->BindAction<FSelectWeaponDelegate>(TEXT("SelectWeapon0"), EInputEvent::IE_Pressed, this, &APawnJerry::SelectWeapon, 0);
		PlayerInputComponent->BindAction(TEXT("FirePrimary"), EInputEvent::IE_Pressed, this, &APawnJerry::BeginFire);
		PlayerInputComponent->BindAction(TEXT("FirePrimary"), EInputEvent::IE_Released, this, &APawnJerry::StopFire);
		PlayerInputComponent->BindAction<FDodgeDelegate>(TEXT("DodgeForward"), EInputEvent::IE_Pressed, this, &APawnJerry::Dodge, 1);
		PlayerInputComponent->BindAction<FDodgeDelegate>(TEXT("DodgeLeft"), EInputEvent::IE_Pressed, this, &APawnJerry::Dodge, 2);
		PlayerInputComponent->BindAction<FDodgeDelegate>(TEXT("DodgeBackward"), EInputEvent::IE_Pressed, this, &APawnJerry::Dodge, 3);
		PlayerInputComponent->BindAction<FDodgeDelegate>(TEXT("DodgeRight"), EInputEvent::IE_Pressed, this, &APawnJerry::Dodge, 4);
		PlayerInputComponent->BindAction(TEXT("OpenMenu"), EInputEvent::IE_Pressed, this, &APawnJerry::OpenMenu);
		PlayerInputComponent->BindAction(TEXT("NextSkill"), EInputEvent::IE_Pressed, this, &APawnJerry::NextSkill);
		PlayerInputComponent->BindAction(TEXT("PreviousSkill"), EInputEvent::IE_Pressed, this, &APawnJerry::PreviousSkill);
		PlayerInputComponent->BindAction(TEXT("DoSkill"), EInputEvent::IE_Pressed, this, &APawnJerry::ExecuteSkill);
	}
}

void APawnJerry::MoveForward(float AxisValue)
{
	AddMovementInput(GetActorForwardVector() * AxisValue);
}

void APawnJerry::Strafe(float AxisValue)
{
	AddMovementInput(GetActorRightVector() * AxisValue);
}

void APawnJerry::Dodge(const int32 Direction)
{
	if (IsDead())
		return;
	if (!CanJump())
		return;
	if (GetWorld()->GetTimeSeconds() - LastDodgeTime >= TimeBetweenDodges) {	// Checks to see if enough time has passed to perform a dodge
		if (GetWorld()->GetTimeSeconds() - LastDirInputTime <= DoublePressTime) {	// Checks to see if two direction inputs occur in rapid succession
			if (Direction == prevDir) {		// If the previous input direction is the same as the current one...
				LastDodgeTime = GetWorld()->GetTimeSeconds();
				FVector DodgeDirection;
				switch (Direction) {
				case 1:	// W - forward
					DodgeDirection = (GetActorForwardVector() * DodgeStrength) + FVector(0, 0, ZAdd);
					break;
				case 2: // A - left
					DodgeDirection = (-GetActorRightVector() * DodgeStrength) + FVector(0, 0, ZAdd);
					break;
				case 3: // S - backwards
					DodgeDirection = (-GetActorForwardVector() * DodgeStrength) + FVector(0, 0, ZAdd);
					break;
				case 4:	// D - right
					DodgeDirection = (GetActorRightVector() * DodgeStrength) + FVector(0, 0, ZAdd);
					break;
				}
				LaunchCharacter(DodgeDirection, false, false);
			}
		}
	}
	prevDir = Direction;	// Stores the current inputted direction
	LastDirInputTime = GetWorld()->GetTimeSeconds();
}

//Called when player walks over a weapon material or minor objective
//Calls the AddWeaponMaterial in ControllerJerry, and destroys the material
//Or activates the Minor Objective
void APawnJerry::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	if (IsDead())
		return;
	AWeaponMaterial* WeaponMaterial = Cast<AWeaponMaterial>(OtherActor);
	if (IsValid(WeaponMaterial))
	{
		//UE_LOG(LogTemp, Warning, TEXT("Player walked over a Weapon Material!"));
		AddWeaponMaterial(WeaponMaterial->GetFactory());
		WeaponMaterial->Destroy();
	}
	else
	{	//Not a Weapon Material. Check if we are standing on a Minor Objective
		AMinorObjective* MinorObjective = Cast<AMinorObjective>(OtherActor);
		if (IsValid(MinorObjective))
		{
			MinorObjective->SetActivatingPawn(this);
			MinorObjective->BeginActivation();
		}
	}
}

//Called when player walks off a Minor Objective
void APawnJerry::NotifyActorEndOverlap(AActor* OtherActor)
{
	Super::NotifyActorEndOverlap(OtherActor);
	AMinorObjective* MinorObjective = Cast<AMinorObjective>(OtherActor);
	if (IsValid(MinorObjective))
	{
		MinorObjective->StopActivation();
	}
}

float APawnJerry::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float DamageToDo = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	if (IsDead() || bInvulnerable)
		return 0.0;
	//Check friendly fire
	//Checking EventInstigator not null is particularly important since AI controllers can be destroyed before we take damage, e.g. projectiles traveling
	if (IsValid(EventInstigator))
	{
		APawnMonster* Monster = Cast<APawnMonster>(EventInstigator->GetPawn());
		if (IsValid(Monster) && Monster->IsPlayerTeam())	//Friendly AI
			return 0.0;
	}

	DamageToDo = FMath::Min(Health, DamageToDo);
	Health -= DamageToDo;
	if (Health <= 0)
	{
		Died();
	}
	return DamageToDo;
}

void APawnJerry::AddHealth(const float& HealthAdd)
{
	if (Health >= HealthMax)
		return;
	Health += HealthAdd;
	if (Health > HealthMax)
		Health = HealthMax;
}

bool APawnJerry::IsDead() const
{
	return Health <= 0;
}

//Called by ObjectiveWeapon when a player has completed a Weapon Objective
//Adds the WeaponClass to the WeaponInventory array
void APawnJerry::AddWeapon(TSubclassOf<AWeapon> WeaponClass)
{
	//Only add if player does not have this weapon
	for (auto wClass: WeaponInventory)
	{
		if (wClass == WeaponClass)
		{
			return;
		}
	}
	WeaponInventory.Emplace(WeaponClass);
}

//Called when a player presses a number on the keyboard
//Checks if the player has the desired weapon
//If so, destroy the weapon being currently held and spawn an instance of the desired weapon
void APawnJerry::SelectWeapon(const int32 WeaponNumber)
{
	if (IsDead())
		return;
	//Check if the currently held weapon is already selected
	if (IsValid(Weapon))
	{
		if (Weapon->GetWeaponNumber() == WeaponNumber)
			return;
	}
	for (auto wClass: WeaponInventory)
	{
		if (IsValid(wClass))
		{
			if (wClass->GetDefaultObject<AWeapon>()->GetWeaponNumber() == WeaponNumber)	//wClass is a TSubclassOf template. Need to get the actual weapon class using GetDefaultObject
			{
				//Destroy the weapon the Weapon pointer is currently pointing to
				if (IsValid(Weapon))
				{
					Weapon->StopFire();
					Weapon->Destroy();
				}
				//Spawn an instance of the selected Weapon
				Weapon = GetWorld()->SpawnActor<AWeapon>(wClass);
				if (IsValid(Weapon))
				{
					Weapon->SetOwner(this);
					if (IsValid(AmmoComponent))
						Weapon->SetAmmoComponent(AmmoComponent);
					Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket"));
					if (bAmplified)
					{
						ASkill_Amplify* Amplify = Cast<ASkill_Amplify>(Skill);
						if (IsValid(Amplify))
						{
							Weapon->SetFireRate(Weapon->GetFireRate() * Amplify->GetFireRateMultiplier());
							Weapon->SetAmmoPerFire(0);
						}
					}
					return;
				}
			}
		}
	}
}

bool APawnJerry::HasWeapon(const int& WeaponNum) const
{
	for (auto wClass: WeaponInventory)
	{
		if (IsValid(wClass))
		{
			if (wClass->GetDefaultObject<AWeapon>()->GetWeaponNumber() == WeaponNum)
				return true;
		}
	}
	return false;
}

int APawnJerry::GetSelectedWeapon() const
{
	if (!IsValid(Weapon))
		return -1;
	return Weapon->GetWeaponNumber();
}

//Called when a player clicks with left mouse
//Input binding is used here, as SetupPlayerInputComponent is called before BeginPlay and Weapon will be null
void APawnJerry::BeginFire()
{
	if (IsDead())
		return;
	if (IsValid(Weapon))
		Weapon->BeginFire();
}

//Called when a player releases left mouse
//Input binding is used here, as SetupPlayerInputComponent is called before BeginPlay and Weapon will be null
void APawnJerry::StopFire()
{
	if (IsValid(Weapon))
		Weapon->StopFire();
}

bool APawnJerry::IsFiringMelee() const
{
	if (!IsValid(Weapon))
		return false;
	return Weapon->IsMelee() && Weapon->IsFiring();
}

//Called when player walks over a material
//Adds the material to the MaterialInventory array
void APawnJerry::AddWeaponMaterial(AFactory_Material* MaterialFactory)
{
	if (MaterialFactory != nullptr)
	{
		MaterialInventory.Emplace(MaterialFactory);
		//UE_LOG(LogTemp, Warning, TEXT("Added Material %d to Inventory!"), WeaponMaterialNumber);
	}
}

//Called by CraftingPost when player walks into crafting location
void APawnJerry::CollectMaterials()
{
	MaterialInventory.Empty();
}

void APawnJerry::OpenMenu()
{
	AControllerJerry* PC = Cast<AControllerJerry>(GetController());
	if (IsValid(PC))
		PC->OpenMenu();
}

void APawnJerry::AddEnergyHelper()
{
	AddEnergy(EnergyRegenAmount);
}

void APawnJerry::AddEnergy(const int& Amount)
{
	Energy += FMath::Min(Amount, EnergyMax - Energy);
}

void APawnJerry::NextSkill()
{
	SkillIndex++;
	if (SkillIndex >= Skills.Num())
		SkillIndex = 0;
}

void APawnJerry::PreviousSkill()
{
	SkillIndex--;
	if (SkillIndex < 0)
		SkillIndex = Skills.Num() - 1;
}

void APawnJerry::ExecuteSkill()
{
	if (IsValid(Skill))	//Already have a running skill
	{
		return;
	}
	if (Energy < EnergyMax)
		return;
	if (Skills.Num() == 0)
	{
		return;
	}
	Skill = GetWorld()->SpawnActor<ASkill>(Skills[SkillIndex]);
	if (!IsValid(Skill))
		return;
	Skill->SetOwner(this);
	Skill->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform);
	Skill->Execute();
	Energy = 0;	//Could use skill energy cost instead, but for player let's just reset energy to 0
}

void APawnJerry::Died()
{
	if (IsValid(GetCapsuleComponent()))
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	if (IsValid(Weapon))
		Weapon->Destroy();
	Weapon = nullptr;
	ATomAndJerryGameModeBase* Game = Cast<ATomAndJerryGameModeBase>(GetWorld()->GetAuthGameMode());
	if (IsValid(Game))
		Game->DecrementNumLives();

	//Iterate over Materials array, and call Spawn on the factories
	for (auto Factory : MaterialInventory)
	{
		if (IsValid(Factory))
			Factory->SpawnMaterial();
	}
	
	if (IsValid(Game))
	{
		if (Game->GetNumLives() == 0)
		{
			Game->EndGame(false);
		}
	}

	float SpawnDelay;
	if (IsValid(Game))
		SpawnDelay = Game->GetPlayerSpawnDelay();
	else
		SpawnDelay = 3.0;
	GetWorldTimerManager().SetTimer(DestroyTimer, this, &APawnJerry::DestroyHelper, SpawnDelay, false, SpawnDelay);
}

void APawnJerry::DestroyHelper()
{
	GetWorldTimerManager().ClearTimer(DestroyTimer);
	Destroy();
}

void APawnJerry::Destroyed()
{
	Super::Destroyed();
	WeaponInventory.Empty();
	MaterialInventory.Empty();
	Skills.Empty();
	ATomAndJerryGameModeBase* Game = Cast<ATomAndJerryGameModeBase>(GetWorld()->GetAuthGameMode());
	if (IsValid(Game))
	{
		uint8 NumLives = Game->GetNumLives();
		if (NumLives > 0)
		{
			Game->RestartPlayer(GetWorld()->GetFirstPlayerController());
		}
	}
}